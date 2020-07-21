package de.hhn.mebike.model;

import java.io.Serializable;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.ManyToOne;


@Entity
public class Tour implements Serializable {

    @Id
    @GeneratedValue
    private long id;

    @ManyToOne
    private Client client;

    public Tour() {
    }

    public long getId() {
        return id;
    }

    public Tour setId(long id) {
        this.id = id;
        return this;
    }

    public Client getClient() {
        return client;
    }

    public Tour setClient(Client client) {
        this.client = client;
        return this;
    }

    @Override
    public String toString() {
        return "Tour{" +
                "id=" + id +
                ", client=" + client +
                '}';
    }
}
