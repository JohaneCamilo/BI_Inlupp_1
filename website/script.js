let tabledata = document.getElementById('tabledata')

setInterval(() => {
    tabledata.innerHTML = "";

    fetch("https://camilofunctionapp.azurewebsites.net/api/GetFromSql?")
    .then(res => res.json())
    .then(data => {
        for(let row of data) {
            tabledata.innerHTML += `<tr><td>${row.deviceid}</td><td>${row.deviceid}</td><td class="time">${(new Date(row.timesent * 1000)).toLocaleString()}</td><td>${row.temperature}</td><td>${row.humidity}</td></tr>`
        }
    })
}, 5000);