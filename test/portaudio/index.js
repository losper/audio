const audio=require("audio");
const fs=require('fs');
var buf=fs.readFileSync('test.pcm');
audio.fingerprint(buf,8);