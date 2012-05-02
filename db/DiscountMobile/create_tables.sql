CREATE TABLE proxy_server (
    proxy_id integer NOT NULL,
    proxy_type integer NOT NULL,
    proxy_host inet NOT NULL,
    proxy_port integer NOT NULL,
    proxy_user character varying(80),
    proxy_password character varying(80),
    CONSTRAINT proxiid_pk PRIMARY KEY (proxy_id)
);

CREATE SEQUENCE proxyserver_proxyid_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;

ALTER TABLE proxy_server ALTER COLUMN proxy_id SET DEFAULT nextval('proxyserver_proxyid_seq'::regclass);

CREATE TABLE requests (
    request_id integer NOT NULL,
    request_url character varying(255) NOT NULL,
    request_uid uuid NOT NULL,
    request_xml character varying,
    request_rmk integer DEFAULT 1 NOT NULL,
    request_command integer,
    CONSTRAINT requestid_pk PRIMARY KEY (request_id),
    CONSTRAINT requestuid_uq UNIQUE (request_uid)
);

CREATE SEQUENCE requests_requestid_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;

ALTER TABLE requests ALTER COLUMN request_id SET DEFAULT nextval('requests_requestid_seq'::regclass);

CREATE TABLE discount_mobile_config (
    system_member integer NOT NULL,
    system_ipv4_host inet DEFAULT '127.0.0.1'::inet NOT NULL,
    system_ipv6_host inet DEFAULT '::1'::inet NOT NULL,
    system_reply_timeout integer DEFAULT 1 NOT NULL,
    system_attempt_count integer DEFAULT 5 NOT NULL,
    system_attempt_timeout integer DEFAULT 5 NOT NULL,
    discount_mobile_sertify character varying(255),
    system_url character varying(255) DEFAULT 'http://www.discountmobile.mobi'::character varying,
    use_proxy integer DEFAULT 0 NOT NULL,
    system_proxy integer,
    system_port integer NOT NULL,
    CONSTRAINT systemproxy_fk FOREIGN KEY (system_proxy) REFERENCES proxy_server(proxy_id)
);

CREATE TABLE local_server_config (
    server_host inet DEFAULT '127.0.0.1'::inet NOT NULL,
    server_port integer DEFAULT 10065 NOT NULL,
    server_attempt_count integer DEFAULT 1 NOT NULL,
    server_attempt_timeout integer DEFAULT 5 NOT NULL
);

