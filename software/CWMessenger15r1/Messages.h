
char contestExchanges[MAXMESSAGES + 1][MAXMESSAGELENGTH + 1] = {    // Exchange Message for Contest or common info:
  "                              ",   // DO NOT CHANGE. 30 spaces to create 1-based array and used for erasing messages
  "CQ CQ CQ DE W8TEE",                // General CQ. Change call as needed
  "599 OH 5NN OH",                           // DX CW contest exchange
  "1D OH",                            // Field Day See: http://www.arrl.org/files/file/Field-Day/2016/2016%20Rules.pdf
  " A W8TEE 54 OH",                   // SSCW This message is preceeded by an incrementing QSO number:http://www.arrl.org/sweepstakes
  "SKN 12345",                        // Straight Key number. Send this before RST report. Might add member number
  "CINCINNATI, OH",                   // QTH
  "NAME JACK JACK",
  "RIG HB 5W QRP",
  "ANT EFHW SLOPE to 60 FT",
  "WX HR IS "                         // NOTE: No comma after last entry, compiler automatically allocates enough memory
};
