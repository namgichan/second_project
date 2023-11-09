// firebase-admin 초기화
const admin = require("firebase-admin");

const serviceAccount = require("./esp8266firebase-a017f-firebase-adminsdk-sn9hp-b84d030b02.json");
admin.initializeApp({
  credential: admin.credential.cert(serviceAccount),
  databaseURL: "https://esp8266firebase-a017f-default-rtdb.asia-southeast1.firebasedatabase.app/"
});


const { initializeApp } = require ("firebase/app");
const { getAuth } = require("firebase/auth");

const firebaseConfig = {
  apiKey: "AIzaSyCxl-0pbCJNOITw4IofyptWCpmbXVuZH5I",
  authDomain: "esp8266firebase-a017f.firebaseapp.com",
  databaseURL: "https://esp8266firebase-a017f-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "esp8266firebase-a017f",
  storageBucket: "esp8266firebase-a017f.appspot.com",
  messagingSenderId: "224421342334",
  appId: "1:224421342334:web:7399eab88905df8ca97c2f",
  measurementId: "G-BF3V0PYXCK"
};

const db = admin.database(); // 내 파이어베이스 프로젝트의 RTDB 참조자 생성 

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const auth = getAuth(app);

module.exports = {
  db, auth
};