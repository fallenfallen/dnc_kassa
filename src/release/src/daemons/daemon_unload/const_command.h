 
const int COMMAND_PARAM_COUNT_MAX = 4;
const int PARAM_STR_LEN_MAX = 300;
const uint PARAM_LEN_MAX = 30;

const char SYMBOL_BEFORE_PARAM = '/';

const int COMMAND_COUNT = 15;
    
const char* COMMANDS[COMMAND_COUNT] = { "ALLTRANSACTIONS",
                                        "TRANSACTIONSBYTRANSRANGE",
                                        "TRANSACTIONSBYCHECKRANGE",
                                        "NEWTRANSACTIONS",
                                        "TRANSACTIONSBYREPORTRANGE",
                                        "TRANSACTIONSBYSESSIONSRANGE",
                                        "TRANSACTIONSBYDATETIMERANGE",
                                        "CURSESSIONTRANSACTIONS",
                                        "LASTSESSIONTRANSACTIONS",
                                        "NEWSALES",
                                        "SALESOFTIMERANGE",
                                        "ALLSALES", 
                                        "SALESOFLASTSESSION",
                                        "SALESOFTRANSRANGE",
                                        "SALESOFSESSIONRANGE"};

//-----(*)-----Zuskin-----25/11/2011-----                                        
//const char* SQLTEXT  = "SELECT n_transaction, tr_type, tr_date_time, n_pc, n_kkm, seller, n_session, n_check, check_type, code, code_tax_group, code_tax_rate, payment_type, card_code, value_1, value_2, reduction_type, reduction_info, auto_scheme_id, gain, ROUND(value_1*value_2,2), barcode, n_doc, value_2 + round_sum, round_sum FROM Transaction " ;
const char* SQLTEXT  = "SELECT n_transaction, tr_type, tr_date_time, n_pc, n_kkm, seller, n_session, n_check, check_type, code, code_tax_group, code_tax_rate, payment_type, card_code, value_1, value_2, reduction_type, reduction_info, auto_scheme_id, gain, ROUND(value_1*value_2,2), barcode, n_doc, value_2 + round_sum, round_sum, manager FROM Transaction " ;
//---------------------------------------

const char* COMMANDS_SQLTEXT[COMMAND_COUNT] = { "WHERE n_pc = ",
                                             "WHERE n_transaction BETWEEN /1                   AND /2                  AND n_pc = ",
                                             "WHERE n_check BETWEEN /1                   AND /2                  AND n_pc = ",
                                             "WHERE n_pc = ",
                                             "WHERE n_check BETWEEN /1                   AND /2                  AND n_pc = ",
                                             "WHERE n_session BETWEEN /1                   AND /2                  AND n_pc = ",
                                             "WHERE tr_date_time BETWEEN CAST('/1                   ' AS TIMESTAMP ) AND CAST('/2                   ' AS TIMESTAMP ) AND n_pc = ",
                                             "t, (SELECT MAX(n_transaction) as max FROM transaction WHERE tr_type = 62) s WHERE t.n_transaction >= s.max AND n_pc = ",
                                             ", ( SELECT MAX(n_transaction) as close_session FROM transaction WHERE tr_type = 61 ) s1, ( SELECT MAX(n_transaction) as open_session FROM transaction, (SELECT MAX(n_transaction) as x FROM transaction WHERE tr_type = 61) s  WHERE tr_type = 62 AND n_transaction < s.x  ) s2 WHERE n_transaction BETWEEN s2.open_session AND s1.close_session AND n_pc = "};  

                                 
                                             

