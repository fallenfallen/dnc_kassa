
CREATE TABLE features_sets ( set_id serial , set_name varchar(100) , code varchar(20), CONSTRAINT set_id_pk PRIMARY KEY (set_id));

CREATE TABLE features ( feature_id serial , feature_name varchar(100) , code varchar(20), CONSTRAINT feature_id_pk PRIMARY KEY (feature_id));

CREATE TABLE features_values ( value_id serial , value_value varchar(100) , code varchar(20), feature_id integer, CONSTRAINT value_id_pk PRIMARY KEY (value_id), CONSTRAINT feature_id_fk FOREIGN KEY (feature_id) REFERENCES features (feature_id) ON DELETE CASCADE);


CREATE TABLE features_in_sets (fis_id serial, set_id integer, feature_id integer,
CONSTRAINT fis_id_pk PRIMARY KEY (fis_id),
CONSTRAINT fis_f_id_fk FOREIGN KEY (feature_id) REFERENCES features (feature_id) ON DELETE CASCADE,
CONSTRAINT fis_s_id_fk FOREIGN KEY (set_id) REFERENCES features_sets (set_id) ON DELETE CASCADE
);


CREATE TABLE Auto_Scheme
(
   auto_scheme_id		integer,
   longtitle			varchar(100),
   shortcut			varchar(100),
   is_default			boolean,
   CONSTRAINT	Auto_Scheme_pk PRIMARY KEY(auto_scheme_id)
);



CREATE TABLE Tax_Group
(
   tax_group_id			integer,
   longtitle			varchar(100),
   shortcut			varchar(100),
   CONSTRAINT	Tax_Group_pk PRIMARY KEY(tax_group_id)
);


CREATE TABLE Tax_Rate
(
   tax_rate_id			integer,
   longtitle			varchar(100),
   shortcut			varchar(100),
   value			numeric(5,2),
   CONSTRAINT	Tax_Rate_pk PRIMARY KEY(tax_rate_id)
);


CREATE TABLE Tax_Rate_Group
(
   id				integer,
   base_flag			boolean,
   tax_group_id			integer,
   tax_rate_id			integer,
   order_id			integer, -- ???? � ��������� ����� ��� ???
   CONSTRAINT	Tax_Rate_Group_pk PRIMARY KEY(id),
   CONSTRAINT	Tax_Rate_Group_tax_group_id_fk FOREIGN KEY(tax_group_id) REFERENCES Tax_Group ON DELETE CASCADE,
   CONSTRAINT	Tax_Rate_Group_tax_rate_id_fk FOREIGN KEY(tax_rate_id) REFERENCES Tax_Rate ON DELETE CASCADE
);


CREATE TABLE Ware_Group
(
   group_id			integer,
   longtitle			varchar(100),
   shortcut			varchar(100),
   parent_group_id		integer CONSTRAINT check_parent CHECK ((parent_group_id != group_id) AND (parent_group_id >= 0)),
   group_code 			varchar(20),
   CONSTRAINT	Ware_Group_pk PRIMARY KEY(group_id),
   CONSTRAINT ware_group_fk FOREIGN KEY(parent_group_id) REFERENCES Ware_Group(group_id) ON DELETE SET null
);


CREATE TABLE Ware 
(
   ware_id 			serial,
   internal_id			varchar(20),
   longtitle			varchar(100),
   shortcut			varchar(100),
   price			numeric(10,2),
   min_price			numeric(10,2),
   quantity			numeric(10,3),
   sale_quantity		numeric(10,3),
   flags			smallint,
   best_before			timestamp,
   auto_scheme_id		integer,
   fix_scheme_id		integer,
   group_id			integer,
   tax_group_id			integer,
   features_set_id 		integer,
   article 			varchar(20),
   section 			integer,
   sale_limit_by_time 		integer,	
   CONSTRAINT ware_id_pk PRIMARY KEY(ware_id),
   CONSTRAINT ware_group_id_fk FOREIGN KEY(group_id) REFERENCES Ware_Group(group_id) ON DELETE SET null,
   CONSTRAINT Ware_auto_scheme_id_fk FOREIGN KEY(auto_scheme_id) REFERENCES Auto_Scheme ON DELETE SET NULL,
   CONSTRAINT Ware_tax_group_id_fk FOREIGN KEY(tax_group_id) REFERENCES Tax_Group ON DELETE SET NULL,
   CONSTRAINT features_set_id_fk FOREIGN KEY (features_set_id) REFERENCES features_sets(set_id) ON DELETE SET null
);

CREATE INDEX longtitle_idx ON ware (longtitle);

CREATE TABLE Ware_barcode
(
   	wb_id			serial,
   	internal_id  		varchar(20),
	barcode 		varchar(20),
	coefficient		numeric(10,3),
	main_barcode 		boolean,
	addition_code 		varchar(20),
	CONSTRAINT warebarcode_id PRIMARY KEY (wb_id)
);


/* CREATE INDEX barcode_ids ON Ware(barcode);*/

CREATE TABLE Fix_Reduction
(
        fix_reduction_id serial,
        external_id      integer,
        scheme_id 	 integer,
        longtitle	 varchar(100),
        shortcut	 varchar(100),
        value		 numeric(10,2),
        type	         integer,
        area		 boolean,
        barcode		 varchar(20),
	cardcode	 varchar(350),
	show		 boolean,
	summ		 numeric(10,2),
	maxbonuspercent  numeric(10,2),
	CONSTRAINT	Fix_Reduction_pk PRIMARY KEY(fix_reduction_id)
);

CREATE TABLE Auto_Reduction
(
   auto_reduction_id		integer,
   auto_scheme_id		integer,
   longtitle			varchar(100),
   shortcut			varchar(100),
   value			numeric(10,2),
   type			integer,
   begindate			date,
   enddate			date,
   begintime			time,
   endtime			time,
   weekdays			smallint,
   quantity			numeric(10,3),
   position_sum			numeric(10,2),
   check_sum			numeric(10,2),
   card_prefix_begin		varchar(40),
   card_prefix_end		varchar(40),
   accumulation			numeric(15,2),
--   ident			serial,
--   CONSTRAINT	Reduction_pk PRIMARY KEY(ident),
   CONSTRAINT	Reduction_pk PRIMARY KEY(auto_reduction_id, auto_scheme_id),
   CONSTRAINT	Reduction_fk FOREIGN KEY(auto_scheme_id) REFERENCES Auto_Scheme ON DELETE CASCADE
);

CREATE TABLE Profiles
(
   profile_id			integer,
   profile_name			varchar(100),
   intcode			boolean NOT NULL,
   barcode_scaner		boolean NOT NULL,
   barcode_manual		boolean NOT NULL,
   visual_search		boolean NOT NULL,
   storno			boolean NOT NULL,
   quant_reduction		boolean NOT NULL,
   price_reduction		boolean NOT NULL,
   repeate_pos			boolean NOT NULL,
   cancel_check			boolean NOT NULL,
   red_manual			boolean NOT NULL,
   red_fixed			boolean NOT NULL,
   red_cancel_auto		boolean NOT NULL,
   red_cancel_other		boolean NOT NULL,
   return_by_number		boolean NOT NULL,
   open_return_check		boolean NOT NULL,
   payment_nal			boolean NOT NULL,
   payment_type2		boolean NOT NULL,
   payment_type3		boolean NOT NULL,
   payment_type4		boolean NOT NULL,
   service_report_z		boolean NOT NULL,
   service_report_x		boolean NOT NULL,
   service_sync_time		boolean NOT NULL,
   entering			boolean NOT NULL,
   getting			boolean NOT NULL,
   copy_check			boolean NOT NULL,
   open_box			boolean NOT NULL,
   setup			boolean NOT NULL,
   handle_unload		boolean NOT NULL,
   handle_upload		boolean NOT NULL,
   ps_reconsiliation  		boolean NOT NULL,
   CONSTRAINT profile_id_pk PRIMARY KEY(profile_id)
);


CREATE TABLE Users
(
  user_id			integer,
  user_name			varchar(100),
  shortcut			varchar(100),
  profile_id			integer,
  bar_code			varchar(20),
  card_code			varchar(100),
  userpassword			varchar(30),
  CONSTRAINT user_id_pk PRIMARY KEY(user_id),
  CONSTRAINT users_profile_id_fk FOREIGN KEY(profile_id) REFERENCES Profiles ON DELETE CASCADE
);


CREATE TABLE schemes
(
	scheme_id	serial,
	scheme_name	varchar(20),
	CONSTRAINT	SchemeId PRIMARY KEY(scheme_id)
);


CREATE TABLE Weit_Pattern
(
  wp_id				serial,
  scheme_id 	 		integer,
  pattern			varchar(40),
  prefix			numeric(2,0),
  multiplier			numeric(10,3),
  CONSTRAINT	Weit_Pattern_pk PRIMARY KEY(wp_id),
  CONSTRAINT 	Weit_Pattern_Scheme_pk FOREIGN KEY(scheme_id) REFERENCES schemes ON DELETE CASCADE
);

CREATE TABLE parameters
(
	param_id	integer		unique,
	param_name	varchar(40),
	param_type	varchar(10),
	default_value	varchar(100),
	CONSTRAINT	ParamId PRIMARY KEY(param_id)
);

CREATE TABLE parametersValues
(
	id		serial,
	param_id	integer,
	scheme_id	integer,
	value		varchar(100),
	CONSTRAINT	ParamValueId PRIMARY KEY(id),
	CONSTRAINT	SchemeId FOREIGN KEY(scheme_id) REFERENCES schemes(scheme_id),
	CONSTRAINT	ParamId FOREIGN KEY(param_id) REFERENCES parameters(param_id)
);

CREATE TABLE stringParameters
(
	stringParam_id	integer,
	stringParam_name	varchar(20),
	default_value	varchar(500),
	CONSTRAINT	StringParamId PRIMARY KEY(stringParam_id)
);

CREATE TABLE stringParametersValues
(
	id		serial,
	stringParam_id	integer,
	scheme_id	integer,
	value		varchar(500),
	CONSTRAINT	StringParamValueId PRIMARY KEY(id),
	CONSTRAINT	SchemeId FOREIGN KEY(scheme_id) REFERENCES schemes(scheme_id),
	CONSTRAINT	StringParamId FOREIGN KEY(stringParam_id) REFERENCES stringParameters(stringParam_id)
);

CREATE TABLE activeScheme
(
	activeScheme_id	integer,
	CONSTRAINT	SchemeId FOREIGN KEY(activeScheme_id) REFERENCES schemes(scheme_id)	
);

CREATE TABLE flags
(
	flags		integer
);


CREATE TABLE Documents(
        document_id serial,
        scheme_id 	 integer,
        document_name	 varchar(100),
        document_path	 varchar(500),
   CONSTRAINT	DocumentId PRIMARY KEY(document_id),
   CONSTRAINT	SchemeId FOREIGN KEY(scheme_id) REFERENCES schemes(scheme_id)  
);
  

CREATE TABLE sections (
	section_id      serial,
	section_number  integer,
	head_text       varchar(200),
	bottom_text     varchar(200),
	section_name    varchar(20),
	fiscal_section  boolean,
	section_cash 	numeric(10,2),
	auto_encashment boolean,
	CONSTRAINT section_pk PRIMARY KEY(section_id)
);

CREATE TABLE sale_time_limit (
start_time   time without time zone,
count_time   time without time zone,
id	integer NOT NULL default(1)
);

CREATE OR REPLACE VIEW sale_limit_by_time_view AS
SELECT CAST(now()-start_time as TIME) > count_time AS sale_access_by_time, id AS id FROM sale_time_limit;

CREATE VIEW features_in_set_view AS
SELECT fis.fis_id, f.feature_id, f.feature_name, f.code, fis.set_id
FROM features f LEFT JOIN features_in_sets fis ON fis.feature_id = f.feature_id;


CREATE TABLE ware_additions ( addition_id serial, addition_article varchar(100), addition_price numeric(8,2), addition_quantity numeric(8,3), addition_code varchar(20), ware_id integer, CONSTRAINT addition_id_pk PRIMARY KEY (addition_id), CONSTRAINT additions_w_id_fk FOREIGN KEY (ware_id) REFERENCES ware (ware_id) ON DELETE CASCADE);


CREATE TABLE ware_additions_features_values ( features_value_id serial, addition_id integer, value_id integer, 
CONSTRAINT wafv_features_value_id_pk PRIMARY KEY (features_value_id), 
CONSTRAINT wafv_addition_id_fk FOREIGN KEY (addition_id) REFERENCES ware_additions (addition_id) ON DELETE CASCADE, 
CONSTRAINT wafv_value_id_fk FOREIGN KEY (value_id) REFERENCES features_values(value_id) ON DELETE CASCADE);



CREATE OR REPLACE VIEW wa_fv_view AS
SELECT fv.features_value_id, fv.addition_id, v.feature_id, fv.value_id, v.value_value, v.code, a.addition_code
FROM ware_additions_features_values fv, features_values v, ware_additions a
WHERE fv.value_id = v.value_id AND fv.addition_id = a.addition_id;


CREATE VIEW additions_set_of_value AS
select distinct fv.addition_id, ' (' || array_to_string(array(SELECT arr.value_value FROM wa_fv_view arr WHERE arr.addition_id = fv.addition_id), ', ' ) || ') ' as set_of_value FROM wa_fv_view fv;

CREATE OR REPLACE VIEW visual_search_view AS
SELECT w.ware_id, w.internal_id, w.longtitle, w.shortcut, NULL::unknown AS addition_code, w.price, w.quantity, wb.barcode FROM ware w LEFT JOIN ware_barcode wb ON wb.internal_id::text = w.internal_id::text
UNION
SELECT w.ware_id, w.internal_id, w.longtitle || sov.set_of_value as longtitle, w.shortcut || sov.set_of_value as shortcut, a.addition_code, a.addition_price AS price, a.addition_quantity AS quantity, ab.barcode FROM ware_additions a LEFT JOIN ware w ON a.ware_id = w.ware_id LEFT JOIN ware_barcode ab ON ab.addition_code = a.addition_code LEFT JOIN additions_set_of_value sov ON sov.addition_id = a.addition_id;

CREATE TABLE managers 
(
    manager_id 		integer,
    manager_name 	varchar(100),
    name_to_print 	varchar(100),
    barcode 		varchar(20),
    cardcode 		varchar(100),
    CONSTRAINT managers_pk PRIMARY KEY (manager_id)
);













