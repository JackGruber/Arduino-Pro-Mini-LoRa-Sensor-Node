# Arduino Pro Mini TTN LoRaWAN sensor node

<img src="img/case_open.jpg">

`Arduino Pro Mini` TTN LoRaWAN Node with `DHT22` and `RFM95` module powered by an 18650 protected battery.  
The Arduino was converted to [LowPower](https://jackgruber.github.io/2019-12-27-Low-power-Arduino-Pro-Mini/) by desoldering the power LED and the voltage converter.

## Circuit

<img src="img/circuit.jpg">
<img src="gerber/promini_rfm95.jpg">

## Case

<img src="img/case_3d.jpg">

## Diagrams 
<img src="img/value_plot.jpg">

### Battery voltage curve over 12 months with measure and send data all 5 minutes

<img src="img/vcc_plot.jpg">

## TTN payload decoder (v2)

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

## TTN payload decoder (v3)

```javascript
function decodeUplink(input) {
  var decoded = {};
  decoded.vcc = (input.bytes[0] + 200)/100;
  if(input.bytes[1] != 255){
    decoded.humidity = input.bytes[1]; 
    decoded.humidity &= ~(1 << 7);
    if(input.bytes[1] >> 7 == 1) { decoded.humidity +=0.5 }
  }
  if(input.bytes[2] != 255 || input.bytes[3] != 255) decoded.temperature = ((input.bytes[2]<<24>>16 | input.bytes[3]) / 10);
  
  return {
    data: decoded,
    warnings: [],
    errors: []
  };
}
```
