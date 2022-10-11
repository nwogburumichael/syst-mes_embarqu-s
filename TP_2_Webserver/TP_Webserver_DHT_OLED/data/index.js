

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
   //-------------------------------------------------------------
   //
   function button()
   {
      btn = 'LEDonoff=ON';
      if(document.getElementById('btn').innerHTML == 'ON')
      {
        btn = 'LEDonoff=OFF';
      }
      // Envoi d'une donnée dans le serveur
      websock.send(btn);
   }

