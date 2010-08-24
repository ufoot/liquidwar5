
-- DROP TABLE metaserver_list;
CREATE TABLE metaserver_list(
    address VARCHAR(15) NOT NULL,
    port INT NOT NULL,
    game VARCHAR(25) NOT NULL,
    version VARCHAR(25) NOT NULL,
    first_ping INT NOT NULL,
    last_ping INT NOT NULL,
    busy_players INT,
    max_players INT,
    password INT,
    comment VARCHAR(100),
    PRIMARY KEY (address,port)
);

-- DROP TABLE metaserver_chat;
CREATE TABLE metaserver_chat(
    address VARCHAR(15) NOT NULL,
    post_date INT NOT NULL,
    game VARCHAR(25) NOT NULL,
    message TEXT,
    PRIMARY KEY (address,post_date),
    INDEX (post_date),
    INDEX (address)
);

