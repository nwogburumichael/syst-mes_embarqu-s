

   InitWebSocket()
   function InitWebSocket()
   { // Connexionn au Websocket
     websock = new WebSocket('ws://'+window.location.hostname+':81/');
     websock.onmessage = function(evt)
     {   
       //  Récuperation des données recue par le serveur websocket
       JSONobj = JSON.parse(evt.data);
     
         document.getElementById('input').innerHTML = JSONobj.TEMP ;
         

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

