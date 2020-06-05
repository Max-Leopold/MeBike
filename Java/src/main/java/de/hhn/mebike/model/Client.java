package de.hhn.mebike.model;

import java.util.List;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.OneToMany;

import org.springframework.lang.NonNull;

@Entity
public class Client {

    @Id
    @GeneratedValue
    private long clientId;

    @NonNull
    @Column(unique = true)
    private String name;

    @OneToMany
    private List<Tour> tours;

    public Client() {
    }

    public long getClientId() {
        return clientId;
    }

    public void setClientId(long userId) {
        this.clientId = userId;
    }

    public List<Tour> getTours() {
        return tours;
    }

    public void setTours(List<Tour> tours) {
        this.tours = tours;
    }
}
