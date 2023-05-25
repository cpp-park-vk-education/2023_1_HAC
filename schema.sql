CREATE TABLE client (
    user_id     SERIAL          PRIMARY KEY,
    login       VARCHAR(20)     NOT NULL UNIQUE,
    email       VARCHAR(40)     NOT NULL UNIQUE,
    password    VARCHAR(2048)     NOT NULL
);

CREATE TABLE timeseries (
    timeseries_id  SERIAL                        PRIMARY KEY,
    name           VARCHAR(50)                   NOT NULL, 
    date_          TIMESTAMP WITHOUT TIME ZONE,
    param          NUMERIC(15, 2)
);
