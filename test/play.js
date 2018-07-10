const audio=require("audio");
const opt={channels:2,format:8,rate:44100,frames:44100};
var aos=new audio.OutputStream(opt);
const fs=require("fs");
var rs=new fs.ReadStream("test.pcm");
var written=0;
var buf;
var t1=new Date().getTime();
aos.on("drain",function(){
    console.log("drain");
    written=aos.push(buf);
    if(buf.length>written){
	buf=buf.subarray(written);
    }else{
	rs.resume();
    }
})
aos.on("done",function(){
    console.log("play done",(new Date().getTime())-t1);
})
rs.on("data",function(data){
    written=aos.push(data);
    console.log("rs.on",data.length,written,data.length>written);
    if(data.length>written){
    	console.log("pause");
    	rs.pause();
    	buf=data.subarray(written);
    }
});
rs.on("end",function(){
    console.log("rs.end");
    rs.close();
    aos.shutdown();
})
