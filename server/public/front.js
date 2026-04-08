const ws = new WebSocket('ws://172.29.18.255:2000');
ws.onopen = () => console.log('WebSocket connecté !');
let temps = null;
let totalTime = 0;
const Graph = document.getElementById('monGraphe').getContext('2d');
const chart = new Chart(Graph, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: 'Fréquence du Générateur (Hz)',
            data: [],
            borderColor: '#00ff00',
            backgroundColor: 'rgba(0, 255, 0, 0.2)',
            tension: 0.2
        }]
    },
    options: { 
        animation: false,
        scales: {
            x: { 
                title: { display: true, text: 'Délai (s)', color: 'white' },
                ticks: { color: '#cccccc' },
                grid: { color: '#333333' }
            },
            y: { 
                title: { display: true, text: 'Fréquence (Hz)', color: 'white' },
                ticks: { color: '#cccccc' },
                grid: { color: '#333333' }
            }
        },
        plugins: {
            legend: { labels: { color: 'white' } }
        }
    }
});

ws.onmessage = (event) => {
    const value = parseFloat(event.data);
    if (isNaN(value)) return;
    const mtn = Date.now();
    if (temps === null) {
        temps = mtn;
        totalTime = 0;
        chart.data.labels.push(totalTime.toFixed(3)); // 3 chiffre apres la virgule
        chart.data.datasets[0].data.push(value);
        chart.update();
        return;
    }
    const seconde = (mtn - temps) / 1000;
    temps = mtn;
    totalTime += seconde;
    const label = totalTime.toFixed(3); // 3 chiffre apres la virgule
    chart.data.labels.push(label);
    chart.data.datasets[0].data.push(value);




    if (chart.data.labels.length > 30) { // Limite à 30 points
        chart.data.labels.shift(); // Supprime le plus ancien label
        chart.data.datasets[0].data.shift(); // Supprime la valeur correspondante
    }

    chart.update();
};
