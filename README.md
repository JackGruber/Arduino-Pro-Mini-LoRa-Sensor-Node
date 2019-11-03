# TTN DHT22 Sensor

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

