package com.example.mqtt;

import java.io.Serializable;

public class Model implements Serializable {

    private static final long serialVersionUID = 1L;

    private String msg;

    public Model() {
    }

    public Model(String msg) {
        this.msg = msg;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }
}
