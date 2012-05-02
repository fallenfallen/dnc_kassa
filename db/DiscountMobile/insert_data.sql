COPY discount_mobile_config (system_member, system_ipv4_host, system_ipv6_host, system_reply_timeout, system_attempt_count, system_attempt_timeout, discount_mobile_sertify, system_url, use_proxy, system_port) FROM stdin;
0	127.0.0.1	::1	1	3	3		http://discountmobile.mobi/src/s.php	0	5454
\.

COPY local_server_config (server_host, server_port, server_attempt_count, server_attempt_timeout) FROM stdin;
127.0.0.1	10065	5	3
\.

COPY proxy_server (proxy_id, proxy_type, proxy_host, proxy_port, proxy_user, proxy_password) FROM stdin;
1	3	127.0.0.1	0		
\.
