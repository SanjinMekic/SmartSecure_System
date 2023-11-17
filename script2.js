// Import the functions you need from the SDKs you need
import { initializeApp } from "https://www.gstatic.com/firebasejs/10.5.2/firebase-app.js";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyA7Blc7jBTfiM8M8zNYC6jeKWf9e3q6K2E",
  authDomain: "smart-living-dfd88.firebaseapp.com",
  databaseURL:
    "https://smart-living-dfd88-default-rtdb.europe-west1.firebasedatabase.app",
  projectId: "smart-living-dfd88",
  storageBucket: "smart-living-dfd88.appspot.com",
  messagingSenderId: "972809437301",
  appId: "1:972809437301:web:cf51a9ce180eb4d96db83c",
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

import {
  getDatabase,
  ref,
  child,
  update,
  set,
  get,
} from "https://www.gstatic.com/firebasejs/10.5.2/firebase-database.js";

const db = getDatabase();

var dugmic1 = document.querySelector(".dugmic1");
var dugmic2 = document.querySelector(".dugmic2");
var dugmic3 = document.querySelector(".dugmic3");

var alarmOn;
var lightOn;
var systemOnOf;
var notificationAdded = false;

function controlAlarm() {
  update(ref(db, "data/"), {
    alarm: parseInt(alarmOn),
  }).catch((error) => {
    alert("error" + error);
  });
}

function controlLight() {
  update(ref(db, "data/"), {
    light: parseInt(lightOn),
  }).catch((error) => {
    alert("error" + error);
  });
}

function alarmOff() {
  alarmOn = 0;
  controlAlarm();
}

function ledOff() {
  lightOn = 0;
  controlLight();
}

function updateButtonStates() {
  if (alarmOn === 1) {
    dugmic1.removeAttribute("disabled");
  } else {
    dugmic1.setAttribute("disabled", "true");
  }

  if (lightOn === 1) {
    dugmic2.removeAttribute("disabled");
  } else {
    dugmic2.setAttribute("disabled", "true");
  }
}

// Load function modified to call updateButtonStates
function Load() {
  const dbref = ref(db);

  get(child(dbref, "data/"))
    .then((snapshot) => {
      if (snapshot.exists()) {
        alarmOn = snapshot.val().alarm;
        lightOn = snapshot.val().light;
        if (alarmOn === 1 && lightOn === 1 && !notificationAdded) {
          funkcija();
          notificationAdded = true;
        } else if (!(alarmOn === 1 && lightOn === 1)) {
          notificationAdded = false;
        }
      } else {
        console.log("No data available");
      }

      // Update button states after loading data
      updateButtonStates();
      updateCheckboxState();
    })
    .catch((error) => {
      console.log(error);
    });
}

function funkcija() {
  var currentTime = new Date();
  var dayOfWeek = new Intl.DateTimeFormat("en-US", { weekday: "long" }).format(
    currentTime
  );
  var year = currentTime.getFullYear();
  var hours = currentTime.getHours();
  var minutes = currentTime.getMinutes();
  var seconds = currentTime.getSeconds();

  var formattedTime = `${dayOfWeek}, ${year}, ${hours}:${minutes}:${seconds}`;
  var newTaskDiv = document.createElement("div");
  newTaskDiv.className = "notifikacija";
  newTaskDiv.innerHTML = `
    <p>Motion detected!</p>
    <p>${formattedTime}</p>
    <div class="ugasi">X</div>
  `;
  document.querySelector(".notificationsContainer").append(newTaskDiv);
  saveData();
}

// Event listeners for dugmic1 and dugmic2 modified to check button states
dugmic1.addEventListener("click", () => {
  if (alarmOn === 1) {
    alarmOff();
    updateCheckboxState();
  }
});

dugmic2.addEventListener("click", () => {
  if (lightOn === 1) {
    ledOff();
    updateCheckboxState();
  }
});

function funkcija2() {
  const notificationsContainer = document.querySelector(
    ".notificationsContainer"
  );
  if (
    notificationsContainer.style.display === "none" ||
    notificationsContainer.style.display === ""
  ) {
    notificationsContainer.style.display = "block";
  } else {
    notificationsContainer.style.display = "none";
  }
}

document
  .querySelector(".notificationsContainer")
  .addEventListener("click", function (e) {
    if (e.target.classList.contains("ugasi")) {
      e.target.parentElement.remove();
      saveData();
    }
  });

dugmic3.addEventListener("click", funkcija2);

const switchCheckbox = document.getElementById("switch");
const buttonsContainer = document.querySelector(".buttons");
const notificationsContainer = document.querySelector(
  ".notificationsContainer"
);
const body = document.body;
const initialDisplay = getComputedStyle(buttonsContainer).display;

function updateCheckboxState() {
  const dataRef = ref(db, "data/sistemUkljucen");

  get(dataRef)
    .then((snapshot) => {
      if (snapshot.exists()) {
        const sistemUkljucen = snapshot.val();
        switchCheckbox.checked = sistemUkljucen === 1;

        const bothButtonsDisabled = dugmic1.disabled && dugmic2.disabled;

        if (bothButtonsDisabled) {
          switchCheckbox.disabled = false;
        } else {
          switchCheckbox.disabled = true;
        }

        // Update the UI based on the checkbox state
        if (switchCheckbox.checked) {
          buttonsContainer.style.display = initialDisplay;
          notificationsContainer.style.display = "block";
          body.style.backgroundColor = "rgba(0, 120, 0, 0.659)";
          systemOnOf = 1;
        } else {
          buttonsContainer.style.display = "none";
          notificationsContainer.style.display = "none";
          body.style.backgroundColor = "rgba(120, 0, 0, 0.659)";
          systemOnOf = 0;
        }
      }
    })
    .catch((error) => {
      console.error(error);
    });
}

document.addEventListener("DOMContentLoaded", () => {
  // Call the function to initially set the checkbox state
  updateCheckboxState();
  showTask();
});

switchCheckbox.addEventListener("change", () => {
  // Update the Firebase database with the new systemOnOf value
  updateSystemOnOf(switchCheckbox.checked ? 1 : 0);

  // Update the UI based on the checkbox state
  if (switchCheckbox.checked) {
    buttonsContainer.style.display = initialDisplay;
    notificationsContainer.style.display = "block";
    body.style.backgroundColor = "rgba(0, 120, 0, 0.659)";
    systemOnOf = 1;
  } else {
    buttonsContainer.style.display = "none";
    notificationsContainer.style.display = "none";
    body.style.backgroundColor = "rgba(120, 0, 0, 0.659)";
    systemOnOf = 0;
  }
});

// Function to update systemOnOf in the Firebase Realtime Database
function updateSystemOnOf(value) {
  update(ref(db, "data/"), {
    sistemUkljucen: parseInt(value),
  }).catch((error) => {
    alert("error" + error);
  });
}

function saveData() {
  localStorage.setItem("data", document.getElementById("taskovi").innerHTML);
}

function showTask() {
  document.getElementById("taskovi").innerHTML = localStorage.getItem("data");
}

// Initial button state update
updateButtonStates();
updateCheckboxState();

Load();
window.setInterval(Load, 2000);
