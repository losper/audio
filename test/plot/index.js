const ws=require("websocket");
const pcm=require('./pcm');
const pack=require("pack");

var pis=new pack.inputStream();
var pos=new pack.outputStream();
var inst;

pis.on("data",function(data){
    console.log("!data",data.constructor.name);
	inst.write(data);
});
var wss=ws.createServer(function(c){
    inst=c;
    c.on("data",function(frm){
        var info=new TextDecoder().decode(frm.PayloadData);
        console.log("[ws]"+info);
	    pis.push(pcm.getpcm());
    });
    
});
var http=require("http").createServer(function(req,res){
    var body;
    req.on("data",function(buf){
        if(body==undefined){
            body=buf;
        }else{
            body+=buf;
        }
    });
    req.on("end",function(){
        if(wss.filter(req,res))return ;
    });
});
http.listen(2348);
