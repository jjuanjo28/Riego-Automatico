import { initializeApp } from "firebase/app"
import {getDatabase, ref, onValue, set} from "firebase/database"
import "firebase/database"

const firebaseConfig = {
    apiKey: "AIzaSyBJUUYBsmItZMXOdaKGFYbMjbEnPUkWEHY",
    authDomain: "cultivo-c71e7.firebaseapp.com",
    databaseURL: "https://cultivo-c71e7-default-rtdb.firebaseio.com",
    projectId: "cultivo-c71e7",
    storageBucket: "cultivo-c71e7.appspot.com",
    messagingSenderId: "475243137677",
    appId: "1:475243137677:web:2d676fbe91d88243ccb0c4"
  };

  const app = initializeApp(firebaseConfig);

  const db = getDatabase()

  export { db, ref, onValue, set };