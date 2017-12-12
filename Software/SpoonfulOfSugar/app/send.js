document
    .getElementById("enviar")
    .addEventListener("click", send)

var request = new XMLHttpRequest();

function createAlarms() {

    var div = document.getElementById("Alarms");
    var divValue = document
        .getElementById("alarmsAmount")
        .value;
    divValue = parseInt(divValue, 10);

    div.innerHTML = "";
    console.log(divValue);

    for (var i = 0; i < divValue; i++) {

        var inputWrapper = document.createElement("div");
        var hour = document.createElement("input");
        var minute = document.createElement("input");

        hour.name="hours[]";
        minute.name="minutes[]";
        inputWrapper.appendChild(hour);
        inputWrapper.appendChild(minute);
        div.appendChild(inputWrapper);
    }

}

function send() {
    var medicine = document
        .getElementById("name")
        .value;
    var amount = document
        .getElementById("amount")
        .value;
    var amountAlarms = document
        .getElementById("alarmsAmount")
        .value;

    var hoursAlarm = document.getElementsByName("hours[]");
    var minutesAlarm = document.getElementsByName("minutes[]");
    var alarms =  Array(hoursAlarm.length).fill(0).map(i=>({hour:0,minute:0}));
    hoursAlarm.forEach((input,i)=>alarms[i].hour=input.value);
    minutesAlarm.forEach((input,i)=>alarms[i].minute=input.value);

    console.log(alarms);
    
    request.open("POST", "http://192.168.43.218", true);

    request.send(JSON.stringify({medicine: medicine, amount: amount, amountAlarms: amountAlarms, alarms: alarms}));
}


function alarmsList(){
   
}