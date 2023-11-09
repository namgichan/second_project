const express = require("express")
const server = express();
const bodyParser = require('body-parser')
const { db, auth } = require("./config")
const { createUserWithEmailAndPassword } = require("firebase/auth")
const { signInWithEmailAndPassword } = require("firebase/auth")
const {signOut} = require("firebase/auth");
 
// 바디 파싱
server.use(bodyParser.json());
server.use(bodyParser.urlencoded( { extended : true } ));

const PORT = process.env.PORT || 3000;
const ref = db.ref()

//auth.currentUser 로그인 여부 확인

server.use(express.static(__dirname + "/public"))


server.get("/islogin", (request, response) => {
  response.setHeader("Access-Control-Allow-Origin","http://127.0.0.1:5500")
  response.json(auth.currentUser)
  
})

server.get("/", (request, response) => {
  console.log("!")
  if(auth.currentUser){
    response.sendFile(__dirname + "/public/main.html")

  }else{
    response.redirect("/index")
  }
})


server.get("/get-current", (request, response) => {

  
    ref.once('value', (data) => {
        response.json(data.val());
    });
});

server.get("/update-ledauto/:key", (request, response) => {
  ref.child("ledcontrol").update({
    ledauto : parseInt(request.params.key)
  }, (err) => {
    if(err){
      response.json('Error Occurred : ' + err);
    }
    else{
      response.json('Work Successfully.');
    }
  })
});

server.get("/update-servoauto/:key", (request, response) => {
  ref.child("servocontrol").update({
    servoauto : parseInt(request.params.key)
  }, (err) => {
    if(err){
      response.json('Error Occurred : ' + err);
    }
    else{
      response.json('Work Successfully.');
    }
  })
});

server.get("/update-ledsetval/:key", (request, response) => {
  response.setHeader("Access-Control-Allow-Origin","http://127.0.0.1:5500")
  ref.child("ledcontrol").update({
    ledsetval : parseInt(request.params.key)
  }, (err) => {
    if(err){
      response.json('Error Occurred : ' + err);
    }
    else{
      response.json('Work Successfully.');
    }
  })
});

server.get("/update-servosetval/:key", (request, response) => {
  response.setHeader("Access-Control-Allow-Origin","http://127.0.0.1:5500")

  console.log(auth.currentUser)

  ref.child("servocontrol").update({
    servosetval : parseInt(request.params.key)
  }, (err) => {
    if(err){
      response.json('Error Occurred : ' + err);
    }
    else{
      response.json('Work Successfully.');
    }
  })
});


server.post("/join", function(request, response){
    const { email, password } = request.body // 구조 분해 

    createUserWithEmailAndPassword(auth, email, password)
    .then((userCredential) => {
      
      response.sendFile(__dirname + "/public/index.html")
        
    })
    .catch((error) => {
      
      response.sendFile(__dirname + "/public/join.html")
    });
})


server.get("/login", (request, response) => {
  response.sendFile(__dirname + "/public/index.html")
})

server.post("/login", function(request, response){
    const { email, password } = request.body
    
    signInWithEmailAndPassword(auth, email, password)
    .then((userCredential) => {
     
      response.sendFile(__dirname + "/public/main.html")
      
    })
    .catch((error) => {
      // 로그인 실패... 따라서 로그인 실패라는 메시지가 담긴 페이지 제공하기 
      
      response.sendFile(__dirname + "/public/index.html")
    });
})

server.post("/logout", (req, response) => {
  // 로그아웃 작업 수행
  signOut(auth)
    .then(() => {
      console.log(auth.currentUser)
      response.sendFile(__dirname + "/public/index.html")
      
   
    })
    .catch((error) => {
      console.error('로그아웃 오류:', error);
      response.status(400).json({ error: '로그아웃 실패' });
    });
});




server.post("/user", function(){
    
})

server.all("/*", (request, response) => {
    response.status(404).json({
      "status" : 404,
      "message" : "BAD REQUEST!!"
    });
})
  
server.listen(PORT, () => {
  console.log("Server Listening on PORT:", PORT);
});