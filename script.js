// DOM Elements
const fromSelect = document.getElementById('from');
const toSelect = document.getElementById('to');
const searchBtn = document.getElementById('searchBtn');
const resultsDiv = document.getElementById('results');
const noResultsDiv = document.getElementById('noResults');
const busResultsDiv = document.getElementById('busResults');
const popularRoutesDiv = document.getElementById('popularRoutes');
const mapModal = document.getElementById('mapModal');
const closeModal = document.getElementById('closeModal');
const busTitle = document.getElementById('busTitle');
const mapStatus = document.getElementById('mapStatus');
const busDetails = document.getElementById('busDetails');

// Map variables
let map;
let busMarker;
let routePolyline;

// Default Dhaka coordinates
const DEFAULT_LOCATION = {
    lat: 23.8103,
    lng: 90.4125
};

// Fetch bus data
async function fetchBusData() {
    try {
        const response = await fetch('buses.json');
        if (!response.ok) throw new Error('Network response was not ok');
        return await response.json();
    } catch (error) {
        console.error('Error loading bus data:', error);
        return {
            buses: [],
            locations: [],
            popularRoutes: []
        };
    }
}

// Initialize the page
async function initPage() {
    const busData = await fetchBusData();
    
    // Populate location dropdowns
    busData.locations.forEach(location => {
        const option1 = new Option(location, location.toLowerCase());
        const option2 = new Option(location, location.toLowerCase());
        fromSelect.add(option1);
        toSelect.add(option2);
    });

    // Populate popular routes
    busData.popularRoutes.forEach(route => {
        const [from, to] = route.split(' - ');
        const routeElement = document.createElement('div');
        routeElement.className = 'bg-white p-4 rounded-lg shadow-sm border border-gray-200 text-center hover:shadow-md transition duration-300 cursor-pointer';
        routeElement.innerHTML = `
            <i class="fas fa-route text-blue-500 text-2xl mb-2"></i>
            <h4 class="font-medium">${route}</h4>
        `;
        routeElement.addEventListener('click', () => {
            fromSelect.value = from.toLowerCase();
            toSelect.value = to.toLowerCase();
            searchBtn.click();
        });
        popularRoutesDiv.appendChild(routeElement);
    });

    // Close modal event
    closeModal.addEventListener('click', () => {
        mapModal.classList.add('hidden');
        if (map) {
            map.remove();
            map = null;
        }
    });

    // Set default selections for demo
    fromSelect.value = 'mirpur';
    toSelect.value = 'motijheel';
}

// Search for buses
async function searchBuses(from, to) {
    const busData = await fetchBusData();
    return busData.buses.filter(bus => {
        const route = bus.route.map(loc => loc.toLowerCase());
        const fromIndex = route.indexOf(from);
        const toIndex = route.indexOf(to);
        return fromIndex !== -1 && toIndex !== -1 && fromIndex < toIndex;
    });
}

// Display results
async function displayResults(buses) {
    busResultsDiv.innerHTML = '';

    if (buses.length > 0) {
        buses.forEach(bus => {
            const fromIndex = bus.route.map(loc => loc.toLowerCase()).indexOf(fromSelect.value);
            const toIndex = bus.route.map(loc => loc.toLowerCase()).indexOf(toSelect.value);
            const routeSegment = bus.route.slice(fromIndex, toIndex + 1);

            const busCard = document.createElement('div');
            busCard.className = 'bg-white rounded-lg shadow-md overflow-hidden cursor-pointer hover:shadow-lg transition duration-300';
            busCard.innerHTML = `
                <div class="p-4">
                    <div class="flex justify-between items-start mb-2">
                        <h3 class="text-lg font-bold text-blue-800">${bus.name}</h3>
                        <span class="${bus.type === 'AC' ? 'bg-green-100 text-green-800' : 'bg-yellow-100 text-yellow-800'} text-xs px-2 py-1 rounded-full">${bus.type}</span>
                    </div>
                    <div class="flex items-center text-gray-600 mb-1">
                        <i class="fas fa-route mr-2"></i>
                        <span>${bus.route[0]} - ${bus.route[bus.route.length - 1]}</span>
                    </div>
                    <div class="flex items-center text-gray-600 mb-3">
                        <i class="fas fa-clock mr-2"></i>
                        <span>${bus.schedule.start} - ${bus.schedule.end}</span>
                    </div>
                    <div class="flex justify-between text-sm">
                        <div>
                            <span class="font-medium">Fare:</span> ৳${bus.fare.min}-৳${bus.fare.max}
                        </div>
                        <div>
                            <span class="font-medium">Frequency:</span> ${bus.frequency}
                        </div>
                    </div>
                </div>
                <div class="bg-gray-50 px-4 py-2 border-t border-gray-200">
                    <div class="flex justify-between text-sm">
                        <span class="text-blue-600 font-medium"><i class="fas fa-bus mr-1"></i> Route Segment:</span>
                        <span>${routeSegment.join(' → ')}</span>
                    </div>
                    <button class="track-btn mt-2 text-blue-600 text-sm font-medium hover:underline flex items-center" data-bus-id="${bus.id}">
                        <i class="fas fa-map-marker-alt mr-1"></i> Track Live Location
                    </button>
                </div>
            `;
            
            busResultsDiv.appendChild(busCard);
        });

        // Add event listeners to all track buttons
        document.querySelectorAll('.track-btn').forEach(btn => {
            btn.addEventListener('click', async (e) => {
                e.stopPropagation();
                const busId = btn.getAttribute('data-bus-id');
                const busData = await fetchBusData();
                const bus = busData.buses.find(b => b.id == busId);
                if (bus) {
                    await showBusOnMap(bus);
                }
            });
        });

        resultsDiv.classList.remove('hidden');
        noResultsDiv.classList.add('hidden');
    } else {
        resultsDiv.classList.add('hidden');
        noResultsDiv.classList.remove('hidden');
    }
}

// Initialize Leaflet map
async function initMap() {
    if (map) return;
    
    map = L.map('map').setView([DEFAULT_LOCATION.lat, DEFAULT_LOCATION.lng], 13);
    
    L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
        attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
    }).addTo(map);
}

// Show bus on map with ThingSpeak data
async function showBusOnMap(bus) {
    // Show modal and set title
    busTitle.textContent = `Live Tracking: ${bus.name}`;
    mapModal.classList.remove('hidden');
    
    // Initialize map
    await initMap();
    
    // Clear previous markers/polylines
    if (busMarker) map.removeLayer(busMarker);
    if (routePolyline) map.removeLayer(routePolyline);
    
    // Show loading state
    mapStatus.innerHTML = `<i class="fas fa-spinner loading-spinner mr-2"></i> Fetching live location...`;
    busDetails.innerHTML = '';
    
    try {
        // Fetch data from ThingSpeak
        const response = await fetch(`https://api.thingspeak.com/channels/${bus.thingspeak.channelId}/feeds/last.json?api_key=${bus.thingspeak.readApiKey}`);
        const data = await response.json();
        
        let lat, lng, statusMessage;
        
        if (response.ok && data.field1 && data.field2) {
            // Use real data from ThingSpeak
            lat = parseFloat(data.field1);
            lng = parseFloat(data.field2);
            statusMessage = `Last updated: ${new Date(data.created_at).toLocaleTimeString()}`;
        } else {
            // Use default location if no data available
            lat = DEFAULT_LOCATION.lat;
            lng = DEFAULT_LOCATION.lng;
            statusMessage = "Using default location (no live data available)";
        }
        
        // Add marker for the bus
        busMarker = L.marker([lat, lng], {
            icon: L.divIcon({
                className: 'bus-marker',
                iconSize: [20, 20]
            })
        }).addTo(map);
        
        // Add route polyline if available
        if (bus.routeCoordinates) {
            routePolyline = L.polyline(bus.routeCoordinates, {
                color: '#3b82f6',
                weight: 4,
                opacity: 0.7,
                className: 'route-path'
            }).addTo(map);
            map.fitBounds(routePolyline.getBounds());
        } else {
            map.setView([lat, lng], 14);
        }
        
        // Update status and details
        mapStatus.innerHTML = `<i class="fas fa-check-circle text-green-500 mr-1"></i> ${statusMessage}`;
        
        busDetails.innerHTML = `
            <div class="bg-blue-50 p-2 rounded">
                <div class="font-medium">${bus.name}</div>
                <div>${bus.type} Service</div>
            </div>
            <div class="bg-gray-50 p-2 rounded">
                <div class="font-medium">Route</div>
                <div>${bus.route.join(' → ')}</div>
            </div>
            <div class="bg-green-50 p-2 rounded">
                <div class="font-medium">Schedule</div>
                <div>${bus.schedule.start} - ${bus.schedule.end}</div>
            </div>
            <div class="bg-yellow-50 p-2 rounded">
                <div class="font-medium">Fare</div>
                <div>৳${bus.fare.min} - ৳${bus.fare.max}</div>
            </div>
        `;
        
        // Add popup with bus info
        busMarker.bindPopup(`
            <b>${bus.name}</b><br>
            ${bus.type} Bus<br>
            Status: ${statusMessage}
        `).openPopup();
        
    } catch (error) {
        console.error("Error fetching bus location:", error);
        mapStatus.innerHTML = `<i class="fas fa-exclamation-triangle text-red-500 mr-1"></i> Error fetching live location. Using default location.`;
        
        // Use default location on error
        busMarker = L.marker([DEFAULT_LOCATION.lat, DEFAULT_LOCATION.lng], {
            icon: L.divIcon({
                className: 'bus-marker',
                iconSize: [20, 20]
            })
        }).addTo(map);
        map.setView([DEFAULT_LOCATION.lat, DEFAULT_LOCATION.lng], 13);
    }
}

// Event Listeners
searchBtn.addEventListener('click', async function() {
    const from = fromSelect.value;
    const to = toSelect.value;
    
    if (from && to) {
        if (from !== to) {
            const matchingBuses = await searchBuses(from, to);
            displayResults(matchingBuses);
        } else {
            alert('Starting point and destination cannot be the same');
        }
    } else {
        alert('Please select both starting point and destination');
    }
});

// Initialize the page
document.addEventListener('DOMContentLoaded', initPage);