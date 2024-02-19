import React from 'react';
import { StyleSheet, Text, TouchableOpacity} from 'react-native';

export function ButtonRed(props) {
   
      const { onPress, text } = props

    return (
        <TouchableOpacity
        onPress={onPress}
        style={{
            ...styles.button,
            backgroundColor: "red"
            }}
        >
            <Text
                style= {{
                    textAlign:"center",

                    color:"white",

                }}
            >{text}</Text>
        </TouchableOpacity>
    );
}

export function ButtonOrange(props) {
   
    const { onPress, text } = props

  return (
      <TouchableOpacity
      onPress={onPress}
      style={{
          ...styles.button,
          backgroundColor: "orange"
          }}
      >
          <Text
              style= {{
                  textAlign:"center",

                  color:"white",

              }}
          >{text}</Text>
      </TouchableOpacity>
  );
}


export function ButtonBlue(props) {
   
    const { onPress, text } = props

  return (
      <TouchableOpacity
      onPress={onPress}
      style={{
          ...styles.buttonBlue,
          backgroundColor: "blue"
          }}
      >
          <Text
              style= {{
                  textAlign:"center",
                  color:"#f1f1f1",
              }}
          >{text}</Text>
      </TouchableOpacity>
  );
}

const styles = StyleSheet.create({
    button : {
        alignSelf: "center",
        borderRadius: 10,
        paddingVertical: 15,
        width:200,
        margin:5,
       
    },
    buttonBlue : {
        alignSelf: "center",
        borderRadius: 10,
        paddingVertical: 15,
        width:300,
        margin:5,
       
    }
})

