
/*основные*/ 
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (0,'LearnMode','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (1,'WriteTransaction','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (109,'LimitClientSum','bool', '1');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (110,'MaxMoney','int', '5000');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (2,'DurationSession','int', '12');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (3,'WarnAboutEnd','int', '15');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (4,'AutoBlock','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (5,'AutoBlockTime','int', '15');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (6,'BlockMouse','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (7,'UseOneClick','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (8,'AuthorLackRights','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (9,'AuthorPassword','bool', '1');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (10,'AuthorCardReader','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (11,'AuthorBarCode','bool', '0');

/*регистрация*/
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (12,'SaveAfterClose','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (13,'ShowProduct','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (14,'ErrorMessage','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (15,'GroupRecord','int', '1');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (16,'CloseFindAfter1reg','bool', '1');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (17,'FindAskCount','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (18,'FindSort','int', '1');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (19,'FindRecordCount','int', '100');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (20,'ControlSale','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (21,'ControlBack','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (22,'ControlDouble','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (23,'ControlPriceEdit','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (24,'ControlExparation','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (25,'ControlMinPrice','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (26,'ControlRemains','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (27,'ControlNegativeRemains','int', '0');


/*отчеты*/
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (28,'AutoTakeReport','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (29,'ReportX','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (30,'ReportSection','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (31,'ReportCount','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (32,'ReportByHour','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (33,'ReportCashier','bool', '0');
insert into parameters (param_id,param_name,param_type, default_value) values (108, 'ReportUnload','bool', '0');

/*произвольные скидки и надбавки*/
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (34,'MaxStandDiscount','procent', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (35,'MaxStandIncrease','procent', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (36,'MaxChequeDiscount','procent', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (37,'MaxChequeIncrease','procent', '0');

/*Печать чека*/
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (38,'WordWrap','bool', '1');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (39,'UseSetupPOS','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (40,'PrintTax','bool', '0');


INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (41,'TitlePCName','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (42,'TitleCashierName','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (43,'TitleDocType','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (44,'TitleWorkTime','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (45,'TitleDocNumber','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (46,'TitleSessionNumber','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (47,'TitleSeparator','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (48,'TitleSeparatorType','int', '0');

INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (49,'StandName','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (50,'StandCount','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (51,'StandPrice','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (52,'StandSeparator','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (53,'StandSeparatorType','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (54,'StandNumber','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (55,'StandCode','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (56,'StandBarCode','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (57,'StandTaxRate','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (58,'StandResultTax','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (59,'StandDiscount','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (60,'StandSumDiscount','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (61,'StandResultDiscount','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (62,'StandTotal','bool', '0');

/*Работа с оборудованием*/
/*Дисплей*/
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (63,'StandUpStringRun','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (64,'StandDownStringRun','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (65,'OffUpStringRun','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (66,'OffDownStringRun','int', '0');

/* детализация при регистрации*/
    /*товар*/
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (67,'DisplayProduct','bool', '1');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (68,'DisplayProductName','bool', '1');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (69,'DisplayProductPrice','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (70,'DisplayProductQuantity','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (107,'DisplayProductSum','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (71,'DisplayProductTotal','bool', '1');

INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (72,'DisplayProductNameString','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (73,'DisplayProductPriceString','int', '1');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (74,'DisplayProductTotalString','int', '1');

INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (75,'DisplayProductUpStringRun','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (76,'DisplayProductDownStringRun','int', '0');

	/*скидки*/
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (77,'DisplayDiscount','bool', '1');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (78,'DisplayDiscountName','bool', '1');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (79,'DisplayDiscountSum','bool', '1');

INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (80,'DisplayDiscountNameString','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (81,'DisplayDiscountSumString','int', '1');

INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (82,'DisplayDiscountUpStringRun','int', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (83,'DisplayDiscountDownStringRun','int', '0');
     
/*ККМ*/
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (84,'Synchro','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (85,'AutoSynchro','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (86,'SynchroFrom','int', '20');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (87,'SynchroTo','int', '30');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (88,'NotifySynchroTimeField','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (89,'NotifySynchroMessage','bool', '0');

INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (90,'OpenBoxByRest','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (91,'OpenBoxByCloseCheque','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (92,'OpenBoxByChangeMoney','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (93,'OpenBoxByClearReport','bool', '0');

/*общие скидки*/
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (94,'ResultDiscount','int', '0');
/*1-автоматическая  2-фиксированная  3-по дисконтной карте  4-произвольная*/
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (95,'BaseDiscountOrder','int', '12');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (96,'AutoRecountDiscount','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (97,'WithoutStandDiscount','bool', '0');

/* печать подвала */
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (98,'BottomCountStand','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (99,'BottomCountBuying','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (100,'BottomTaxRates','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (101,'BottomResultTax','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (102,'BottomDiscount','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (103,'BottomSumDiscount','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (104,'BottomResultDiscount','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (105,'BottomSeparator','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (106,'BottomSeparatorType','int', '0');


/*added param*/
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (111,'PrintOneReg', 'bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (112,'ShowAbsentGoods', 'bool', '1');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (113,'ReturnCountInc','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (114,'ReportPSReconsiliation','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (115,'VSManualInput','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (116,'ReportWare','bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (117,'RoundCheckSum', 'bool', '0');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (118,'RoundPower', 'double', '0.5');
INSERT INTO parameters(param_id, param_name, param_type, default_value) VALUES (119,'RoundType', 'int', '0');



/*Длинные параметры*/
INSERT INTO stringParameters(stringParam_id, stringParam_name, default_value)  VALUES (501, 'Advertisement', '********************Место для рекламы*');
INSERT INTO stringParameters(stringParam_id, stringParam_name, default_value)  VALUES (502, 'StandUpString', 'Свободная касса');
INSERT INTO stringParameters(stringParam_id, stringParam_name, default_value)  VALUES (503, 'StandDownString', 'Добро пожаловать!');
INSERT INTO stringParameters(stringParam_id, stringParam_name, default_value)  VALUES (504, 'OffUpString', 'Касса не работает');
INSERT INTO stringParameters(stringParam_id, stringParam_name, default_value)  VALUES (505, 'OffDownString', 'Технический перерыв'); 
INSERT INTO stringParameters(stringParam_id, stringParam_name, default_value)  VALUES (506, 'PicturePath', '/usr/share/dnc/movie/k.GIF');      

    
INSERT INTO schemes (scheme_name) VALUES ('Scheme1');

INSERT INTO parametersValues (param_id, scheme_id, value) SELECT p.param_id, s.scheme_id, p.default_value  FROM parameters p, schemes s WHERE s.scheme_name = 'Scheme1';
INSERT INTO stringParametersValues (stringParam_id, scheme_id, value) SELECT sp.stringParam_id, s.scheme_id, sp.default_value  FROM stringParameters sp, schemes s WHERE s.scheme_name = 'Scheme1';

INSERT INTO activeScheme (activeScheme_id) SELECT scheme_id FROM schemes WHERE scheme_name = 'Scheme1';
INSERT INTO documents (scheme_id, document_name, document_path) SELECT scheme_id, 'Товарный чек', '/usr/share/dnc/print_doc/товарный_чек.html' FROM schemes WHERE scheme_name = 'Scheme1';


