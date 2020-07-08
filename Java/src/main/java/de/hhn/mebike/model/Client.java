package de.hhn.mebike.model;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;

@Entity
public class Client {

    @Id
    @GeneratedValue
    private long clientId;

    public Client() {
    }

    public long getClientId() {
        return clientId;
    }

    public Client setClientId(long clientId) {
        this.clientId = clientId;
        return this;
    }
}
