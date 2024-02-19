import React from 'react';

import { useState, useEffect, useCallback } from "react";
import { StatusBar } from "expo-status-bar";
import { StyleSheet, Text, View, ScrollView } from "react-native";
import { db, ref, onValue, set } from "../../firebaseConfig";
import { Image } from "expo-image";

import nImage from "../../assets/Logo.png";
import motorApagado from "../../assets/motorApagado.png";
import motorPrendido from "../../assets/motorPrendido.png";
import {  ButtonOrange } from "../../src/components/Buttons.js";
import { useFonts } from "expo-font";
import * as SplashScreen from "expo-splash-screen";

import { useStore } from "../../store/useStore";


export default function Home ({navigation})  {

  const porcentajeDeCorte = useStore((state)=> state.porcentajeDeCorte)
  const miliSegundosBomba = useStore((state)=> state.miliSegundosBomba)
  const [humedadSuelo, setHumedadSuelo] = useState(759);
  const [ledRojo, setLedRojo] = useState(false);
  const [bomba, setBomba] = useState(false);
  const [porcentaje, setPorcentaje] = useState(
   // porcentajeSuelo = ((870 - humedadSuelo) / 3.7 - 100 ) * -1;
  (((870 - humedadSuelo) / 3,7 - 100 )* -1).toString()
  // ((humedadSuelo * 100) / 860 - 100).toString()
  );
  const [tiempoBomba, setTiempoBomba] = useState(0);
  const [fonstLoaded] = useFonts({
   Jomhuria: require("../../assets/fonts/Jomhuria-Regular.ttf"),
   // LeagueSpartan: require("../../assets/fonts/LeagueSpartan-Bold.otf")
  });
  const [porcentajeUsuario, setPorcentajeUsuario] = useState(0)
 
  const [horarioFirebase , setHorarioFirebase] = useState("")

  useEffect(() => {
    async function prepare() {
      await SplashScreen.preventAutoHideAsync();
    }
    prepare();
  }, []);

  const onLayout = useCallback(async () => {
    if (fonstLoaded) {
      await SplashScreen.hideAsync();
    }
  }, [fonstLoaded]);

  useEffect(() => {
    const data = ref(db);
    onValue(data, (snapshot) => {
      setHumedadSuelo(snapshot.val().HumedadSuelo.setInt);
      setBomba(snapshot.val().estadoBomba);
      setPorcentaje(
        snapshot.val().HumedadSuelo.porcentaje.toString().slice(-7)
      );
      setTiempoBomba(snapshot.val().tiempoBomba);
      setPorcentajeUsuario(((((parseInt(snapshot.val().porcentajeUsuario) * 100) / 1024) - 100 ) * -1).toString().slice(0,-8))
      setHorarioFirebase(snapshot.val().HoraActual)
    });
  }, [db]);
  if (!fonstLoaded) return null;

  const onOffLed = () => {
    setLedRojo(true);
    let dataLed = 1;
    set(ref(db, "luz"), dataLed);

    setTimeout(() => {
      setLedRojo(false);
      dataLed = 0;
      set(ref(db, "luz"), dataLed);
    }, 55000);
  };

    return (
        <ScrollView>
        
       
    <View style={styles.container} onLayout={onLayout}>
     <Image source={nImage} style={{ width: 200, height: 200 }} />
     {porcentaje == 0 ? (
      <View
        style={{
          ...styles.porcentualBox,
          backgroundColor: "Red",
          borderRadius: 15,
        }}
      >
        <Text style={styles.porcientoText}>Sonda desconectada</Text>
      </View>
    ) : (
      <View
        style={{
          ...styles.porcentualBox,
          backgroundColor: "blue",
          borderRadius: 15,
        }}
      >
        <Text >      Ultimo horario de acceso: {horarioFirebase}</Text>
        <Text >      El porcentaje de corte es:  {porcentajeDeCorte} %</Text>
        <Text style={styles.porcientoText}>{porcentaje} %</Text>
        <Text style={styles.textoParaPorciento}>de humedad del suelo</Text>
      </View>
    )}

    {ledRojo ? (
      <View style={{ flexDirection: "row", marginTop: 15 }}>
        <Text style={{ ...styles.text, marginTop: 20 }}>
          Enviando datos a la bomba:
        </Text>
        <Image
          source={motorPrendido}
          style={{ width: 100, height: 120, marginLeft: 25 }}
        />
      </View>
    ) : (
      <View style={{ flexDirection: "row", marginTop: 15 }}>
        <Text style={{ ...styles.text, marginTop: 20 }}>
          Enviando datos a la bomba:
        </Text>
        <Image
          source={motorApagado}
          style={{ width: 100, height: 120, marginLeft: 25 }}
        />
      </View>
    )}

    {bomba == 1 ? (
      <View style={{ flexDirection: "row" }}>
        <Text style={{ ...styles.text, marginTop: 20 }}>
          Estado de la Bomba:
        </Text>
        <Image
          source={motorPrendido}
          style={{ width: 100, height: 120, marginLeft: 30 }}
        />
      </View>
    ) : (
      <View style={{ flexDirection: "row" }}>
        <Text style={{ ...styles.text, marginTop: 20 }}>
          Estado de la Bomba:
        </Text>
        <Image
          source={motorApagado}
          style={{ width: 100, height: 120, marginLeft: 30 }}
        />
      </View>
    )}

    <ButtonOrange onPress={onOffLed} text={`Fuerzo bomba x ${tiempoBomba/60000} minutos`} />

    <StatusBar style="auto" />
  </View>
  </ScrollView>
    );
}


const styles = StyleSheet.create({
    titulo: {
      fontSize: 60,
      fontFamily: "LeagueSpartan",
    },
    container: {
      flex: 1,
      backgroundColor: "green",
      alignItems: "center",
      justifyContent: "center",
      
    },
    porcientoText: {
      fontSize: 40,
      fontWeight: "bold",
      textAlign: "center",
    },
    porcentualBox: {
      height: 180,
      marginTop: 35,
      paddingTop: 20,
      width: "90%",
    },
    textoParaPorciento: {
      fontSize: 25,
      textAlign: "center",
    },
    text: {
      fontSize: 20,
      width: 130,
      textAlign: "center",
      flexDirection: "row",
      alignItems: "center",
    },
  });
  


