

   InitWebSocket()
   function InitWebSocket()
   { // Connexionn au Websocket
     websock = new WebSocket('ws://'+window.location.hostname+':81/');
     websock.onmessage = function(evt)
     {   
       
       //  Récuperation des données recue par le serveur websocket
        JSONobj = JSON.parse(evt.data);
        console.log(JSONobj.temperature);
        console.log(JSONobj);
      
        document.getElementById('valeur_temp').innerHTML = JSONobj.temperature ;
        document.getElementById('valeur_humidity').innerHTML = JSONobj.humi ;
        document.getElementById('valeur_bp').innerHTML = JSONobj.valeur_bp ;
         

     }
   }
   //-------------------------Envoi de l'état du bouton------------------------------------

   function bouton() {
    var checkBox = document.getElementById("bp_led");
    
    if (checkBox.checked == true){
      websock.send('LEDonoff=ON');
      } 
    else if (checkBox.checked == false){
       websock.send('LEDonoff=OFF');
      }
    }

    function startTime() {
      var today = new Date();
      var h = today.getHours();
      var m = today.getMinutes();
      var d = today.getDate();
      m = checkTime(m);
      document.getElementById('time').innerHTML =
      "Local Time : "+h + ":" + m + ":" + d ;
      var t = setTimeout(startTime, 500);
    }
    function checkTime(i) {
      if (i < 10) {i = "0" + i};  // add zero in front of numbers < 10
      return i;
    }
     
    startTime();
    
    date = new Date();
    year = date.getFullYear();
    month = date.getMonth() + 1;
    day = date.getDate();
    document.getElementById("current_date").innerHTML = month + "/" + day + "/" + year;
    
