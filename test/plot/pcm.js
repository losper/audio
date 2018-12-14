const cp = require('child_process');
const fs = require('fs');
function getpcm(){
    const sp=cp.execSync("ffprobe -v quiet -print_format json -show_format -show_streams ../data/1.wav");

    var obj = JSON.parse(sp.stdout);
    var cn=obj.streams[0].codec_name;
    var sr=obj.streams[0].sample_rate;

    var cmd="ffmpeg -i ../data/1.wav -f "+cn.slice(4)+" -ar "+sr+" -acodec "+cn+" ../data/1.pcm";

    sp=cp.execSync(cmd);
    obj.data=fs.readFileSync("../data/1.pcm");
    return obj;
}
module.exports={
    getpcm:getpcm,
}
