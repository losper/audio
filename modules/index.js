var audio = require("./audio.passoa");
function audioPlay(opt,buf,cb){
    audio.play(opt.seconds,opt.channels,opt.format,opt.rate,opt.frames,buf,cb);
}
function audioRecord(opt,cb){
    console.log("");
}

function RecordStream(opt,cb){
    var inst=audio.open(opt.seconds,opt.channels,opt.format,opt.rate,opt.frames);
    //console.log(__dirname);
    audio.record(inst,cb);
    this.resume=function(){
	
    }
}

function PlayStream(opt){
    var inst=audio.open(opt.seconds,opt.channels,opt.format,opt.rate,opt.frames);
    this.callback=function(){
	console.log("aps.push.callback");
    }
    this.push=function(buf){
	audio.play(inst,buf,this.callback.bind(this));
	
    }
}

module.exports={
    RecordStream:RecordStream,
    PlayStream:PlayStream,
    float32:1,
    int32:2,
    int24:4,
    int16:8,
    int8:16,
    uint8:32,
}
