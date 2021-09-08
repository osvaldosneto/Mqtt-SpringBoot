package com.example.mqtt.controller;

import com.example.mqtt.Model;
import com.example.mqtt.MqttGateway;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class MqttController {

    @Autowired
    MqttGateway mqttGateway;

    @RequestMapping(method = RequestMethod.POST, value = "/sendmsg")
    public ResponseEntity sendMsg(@RequestBody Model msg){
        try {
            mqttGateway.sendToMqtt (msg.getMsg(), "/swa/commands");
            return ResponseEntity.ok("Success");
        } catch(Exception ex) {
            ex.printStackTrace();
            return ResponseEntity.ok("fail");
        }
    }

}
