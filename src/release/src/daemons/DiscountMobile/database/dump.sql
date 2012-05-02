--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: proxy_server; Type: TABLE; Schema: public; Owner: user; Tablespace: 
--

CREATE TABLE proxy_server (
    proxy_id integer NOT NULL,
    proxy_type integer NOT NULL,
    proxy_host inet NOT NULL,
    proxy_port integer NOT NULL,
    proxy_user character varying(80),
    proxy_password character varying(80)
);


ALTER TABLE public.proxy_server OWNER TO "user";

--
-- Name: ProxyServer_ProxyID_seq; Type: SEQUENCE; Schema: public; Owner: user
--

CREATE SEQUENCE "ProxyServer_ProxyID_seq"
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public."ProxyServer_ProxyID_seq" OWNER TO "user";

--
-- Name: ProxyServer_ProxyID_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: user
--

ALTER SEQUENCE "ProxyServer_ProxyID_seq" OWNED BY proxy_server.proxy_id;


--
-- Name: ProxyServer_ProxyID_seq; Type: SEQUENCE SET; Schema: public; Owner: user
--

SELECT pg_catalog.setval('"ProxyServer_ProxyID_seq"', 95, true);


SET default_with_oids = true;

--
-- Name: requests; Type: TABLE; Schema: public; Owner: user; Tablespace: 
--

CREATE TABLE requests (
    request_id integer NOT NULL,
    request_url character varying(255) NOT NULL,
    request_uid uuid NOT NULL,
    request_xml character varying,
    request_rmk integer DEFAULT 1 NOT NULL,
    request_command integer
);


ALTER TABLE public.requests OWNER TO "user";

--
-- Name: Requests_RequestID_seq; Type: SEQUENCE; Schema: public; Owner: user
--

CREATE SEQUENCE "Requests_RequestID_seq"
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public."Requests_RequestID_seq" OWNER TO "user";

--
-- Name: Requests_RequestID_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: user
--

ALTER SEQUENCE "Requests_RequestID_seq" OWNED BY requests.request_id;


--
-- Name: Requests_RequestID_seq; Type: SEQUENCE SET; Schema: public; Owner: user
--

SELECT pg_catalog.setval('"Requests_RequestID_seq"', 15, true);


SET default_with_oids = false;

--
-- Name: discount_mobile_config; Type: TABLE; Schema: public; Owner: user; Tablespace: 
--

CREATE TABLE discount_mobile_config (
    system_member integer NOT NULL,
    system_ipv4_host inet DEFAULT '127.0.0.1'::inet NOT NULL,
    system_ipv6_host inet DEFAULT '::1'::inet NOT NULL,
    system_reply_timeout integer DEFAULT 1 NOT NULL,
    system_attempt_count integer DEFAULT 5 NOT NULL,
    system_attempt_timeout integer DEFAULT 5 NOT NULL,
    discount_mobile_sertify character varying(255),
    use_proxy boolean DEFAULT false NOT NULL,
    system_proxy integer,
    system_port integer NOT NULL,
    system_url character varying(255) DEFAULT 'http://discountmobile.mobi/src/s.php'::character varying
);


ALTER TABLE public.discount_mobile_config OWNER TO "user";

--
-- Name: local_server_config; Type: TABLE; Schema: public; Owner: user; Tablespace: 
--

CREATE TABLE local_server_config (
    server_host inet DEFAULT '127.0.0.1'::inet NOT NULL,
    server_port integer DEFAULT 10065 NOT NULL,
    server_attempt_count integer DEFAULT 1 NOT NULL,
    server_attempt_timeout integer DEFAULT 5 NOT NULL
);


ALTER TABLE public.local_server_config OWNER TO "user";

--
-- Name: proxy_id; Type: DEFAULT; Schema: public; Owner: user
--

ALTER TABLE proxy_server ALTER COLUMN proxy_id SET DEFAULT nextval('"ProxyServer_ProxyID_seq"'::regclass);


--
-- Name: request_id; Type: DEFAULT; Schema: public; Owner: user
--

ALTER TABLE requests ALTER COLUMN request_id SET DEFAULT nextval('"Requests_RequestID_seq"'::regclass);


--
-- Data for Name: discount_mobile_config; Type: TABLE DATA; Schema: public; Owner: user
--

COPY discount_mobile_config (system_member, system_ipv4_host, system_ipv6_host, system_reply_timeout, system_attempt_count, system_attempt_timeout, discount_mobile_sertify, use_proxy, system_proxy, system_port, system_url) FROM stdin;
4158	127.0.0.1	::1	1	3	3		t	95	5454	http%3A%2F%2Fdiscountmobile.mobi%2Fsrc%2Fs.php
\.


--
-- Data for Name: local_server_config; Type: TABLE DATA; Schema: public; Owner: user
--

COPY local_server_config (server_host, server_port, server_attempt_count, server_attempt_timeout) FROM stdin;
0.0.0.0	10066	5	3
\.

--
-- Data for Name: requests; Type: TABLE DATA; Schema: public; Owner: user
--

COPY requests (request_id, request_url, request_uid, request_xml, request_rmk, request_command) FROM stdin;
\.


--
-- Name: ProxiID_PK; Type: CONSTRAINT; Schema: public; Owner: user; Tablespace: 
--

ALTER TABLE ONLY proxy_server
    ADD CONSTRAINT "ProxiID_PK" PRIMARY KEY (proxy_id);


--
-- Name: RequestID_PK; Type: CONSTRAINT; Schema: public; Owner: user; Tablespace: 
--

ALTER TABLE ONLY requests
    ADD CONSTRAINT "RequestID_PK" PRIMARY KEY (request_id);


--
-- Name: RequestUID_UQ; Type: CONSTRAINT; Schema: public; Owner: user; Tablespace: 
--

ALTER TABLE ONLY requests
    ADD CONSTRAINT "RequestUID_UQ" UNIQUE (request_uid);


--
-- Name: SystemProxy_FK; Type: FK CONSTRAINT; Schema: public; Owner: user
--

ALTER TABLE ONLY discount_mobile_config
    ADD CONSTRAINT "SystemProxy_FK" FOREIGN KEY (system_proxy) REFERENCES proxy_server(proxy_id);


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

