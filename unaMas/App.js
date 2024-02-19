import * as React from "react";
import { useEffect,   useCallback } from "react";
import { NavigationContainer } from "@react-navigation/native";
import Home from "./src/screens/Home";
import Configuration from "./src/screens/Configuration";
import { createNativeStackNavigator } from "@react-navigation/native-stack";
import { TouchableOpacity } from "react-native";
import { AntDesign } from '@expo/vector-icons'; 
import { useFonts } from "expo-font";
import * as SplashScreen from "expo-splash-screen";


const Stack = createNativeStackNavigator();

export default function App() {

  const [fonstLoaded] = useFonts({
    Jomhuria: require("./assets/fonts/Jomhuria-Regular.ttf"),
  });


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
  return (
    <NavigationContainer >
      <Stack.Navigator onLayout={onLayout}
      
       initialRouteName="Home"
      screenOptions={{
          headerStyle: {
            backgroundColor: '#3498db', // Puedes cambiar el color de fondo
            
          },
          headerTintColor: 'black', // Puedes cambiar el color del texto
          headerTitleStyle: {
            fontFamily: "Jomhuria",
            fontSize:45
          },
        }}
        
         >
        <Stack.Screen name="      Auto-Riego-App" component={Home} 
           options={({ navigation}) => ({
          headerRight: () => (
            <TouchableOpacity onPress={
              ()=> navigation.navigate("Configuration")
              }>
             <AntDesign name="setting" size={44}/>
            </TouchableOpacity>
          )
        }
        )
        }

        />
        <Stack.Screen name="Configuration" component={Configuration} />
      </Stack.Navigator>
    </NavigationContainer>
  );
}
