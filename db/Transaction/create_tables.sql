
CREATE TABLE Transaction_Type
(
   tr_type			integer,
   name				varchar(60),
   CONSTRAINT	Transaction_Type_pk PRIMARY KEY(tr_type)
);

CREATE TABLE Transaction
(
   n_transaction	serial,
   tr_type		integer,
   tr_date_time		timestamp,
   n_pc			integer,
   n_kkm		integer,
   seller		integer,
   n_session      	integer,
   n_check		integer,
   n_doc		integer,
   check_type		integer,
   code			varchar(20),
   code_tax_group	integer,
   code_tax_rate	integer,
   payment_type		integer,
   card_code		varchar(40),
   value_1		numeric(15,3),
   value_2		numeric(15,3),
   reduction_type	integer,
   reduction_info    	varchar(20),
   auto_scheme_id    	integer,
   gain              	numeric(15,3),
   barcode		varchar(20),
   protokol_type	integer, 
   round_sum		numeric(15,3),
   manager		integer,
   
   CONSTRAINT	Transaction_pk PRIMARY KEY(N_transaction),
   CONSTRAINT	Transaction_fk FOREIGN KEY(tr_type) REFERENCES Transaction_Type
);

CREATE OR REPLACE VIEW transaction_by_last_session AS
SELECT * FROM transaction tr, (SELECT CASE WHEN MAX(n_transaction) IS NULL THEN 0 ELSE MAX(n_transaction) END AS max FROM transaction WHERE tr_type = 61) s WHERE tr.n_transaction > s.max;

CREATE OR REPLACE VIEW transaction_with_closed_check_by_last_session AS
SELECT * FROM transaction_by_last_session tr WHERE tr.n_check IN (SELECT trcc.n_check FROM transaction_by_last_session trcc WHERE trcc.tr_type = 55 AND trcc.tr_date_time = tr.tr_date_time);

