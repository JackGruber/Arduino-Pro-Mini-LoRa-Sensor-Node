# TTN DHT22 Sensor

Temperature and humidity sensor based on an `Arduino Pro Mini` with `DHT22` sensor and `RFM95` LoRa modul which is operated from on 18650 protected battery. 

## Circuit

<img src="img/circuit.jpg">

## Case

<img src="img/case_3d.jpg">

## Diagrams 

<img src="img/value_plot.jpg">

## TTN payload decoder

```javascript
function Decoder(bytes, port) {
  var decoded = {};
  
  decoded.vcc = (bytes[0] + 200)/100;
  
  if(bytes[1] != 255){
    decoded.humidity = bytes[1]; 
    decoded.humidity &= ~(1 << 7);
    if(bytes[1] >> 7 == 1) { decoded.humidity +=0.5 }
  }
  
  if(bytes[2] != 255 || bytes[3] != 255) decoded.temperature = ((bytes[2]<<24>>16 | bytes[3]) / 10);
  return decoded;
}
```
