import React, { useState, useEffect } from "react";
import { View, StyleSheet, Text, TextInput, SafeAreaView } from "react-native";
import { db, ref, onValue, set } from "../../firebaseConfig";
import { useStore } from "../../store/useStore";
import { ButtonRed, ButtonBlue } from "../components/Buttons";
import { createNativeStackNavigator } from "@react-navigation/native-stack";
import { LinearGradient } from 'expo-linear-gradient';
const Stack = createNativeStackNavigator();
const Configuration = ({navigation}) => {
  const [humedadSuelo, setHumedadSuelo] = useState(750);
  const [tiempoBomba, setTiempoBomba] = useState(60000);

  const [porcentaje, setPorcentaje] = useState(
    (((870 - humedadSuelo) / 3,7 )).toString()
    
  );

  const porcentajeDeCorte = useStore((state) => state.porcentajeDeCorte);
  const subePorcentajeDeCorte = useStore((state) => state.subePorentajeDeCorte);
  const bajaPorentajeDeCorte = useStore((state) => state.bajaPorentajeDeCorte);

  const miliSegundosBomba = useStore((state) => state.miliSegundosBomba);

  const agregaSegundosBomba = useStore((state) => state.agregaSegundosBomba);
  console.log("soy milisegundosBomba:", miliSegundosBomba);
  useEffect(() => {
    const data = ref(db);
    onValue(data, (snapshot) => {
      setHumedadSuelo(snapshot.val().HumedadSuelo.setInt);
      setPorcentaje(
        snapshot.val().HumedadSuelo.porcentaje.toString().slice(-7)
      );
      setTiempoBomba(snapshot.val().tiempoBomba);
      // setPorcentajeUsuario(snapshot.val().porcentajeUsuario);
    });
  }, [db]);
  useEffect(() => {
    if (!isNaN(tiempoBomba)) {
      set(ref(db, "tiempoBomba"), tiempoBomba);
    }
  }, [tiempoBomba]);

  useEffect(() => {
    console.log("soy porcentaje de corte:", porcentajeDeCorte)
    let algo = (870 - (porcentajeDeCorte * 3.7));
    console.log("soy algo:", algo)
    set(ref(db, "porcentajeUsuario"), parseInt(algo));
  }, [porcentajeDeCorte]);
  useEffect(() => {
    set(ref(db, "tiempoBomba"), parseInt(miliSegundosBomba));
  }, [miliSegundosBomba]);

  const setSegundosBomba = (text) => {
    const segundos = text.trim() === "" ? 0 : parseInt(text, 10)*1000;
    if (!isNaN(segundos)) {
      agregaSegundosBomba(segundos*60);
    }
  };

  return (
    <View>
      <Text>Soy configuration Page</Text>
      <Text>Soy el % del Store: {porcentajeDeCorte}</Text>
      <View>
        <ButtonBlue text={"incrementa 1 %"} onPress={subePorcentajeDeCorte} />
        <ButtonBlue text={"decrementa 1 %"} onPress={bajaPorentajeDeCorte} />
      </View>
      <Text>soy miliSegundosBomba: {miliSegundosBomba}</Text>
      <View style={{ alignItems: "center", flex:0, flexDirection:"row" }}>
        <Text>Ingresa los minutos de forzado :  </Text>
        <TextInput
          style={{
            borderStyle: "solid",
            width: "20%",
            borderWidth: 2,
            
          }}
          onChangeText={(text) => setSegundosBomba(text)}
          keyboardType="numeric"
        />
      </View>
      <ButtonRed text="Guardar la nueva configuracion" onPress={()=> navigation.navigate("      Auto-Riego-App")} />
      <LinearGradient
        // Background Linear Gradient
        colors={['rgba(0,0,0,0.8)', 'transparent']}
        style={styles.background}
      />
     
    </View>
  );
};

const styles = StyleSheet.create({

  
});

export default Configuration;
