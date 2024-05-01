    AREA    .text, CODE, READONLY
    EXPORT  set_uci
    EXPORT  clear_uci

set_uci PROC
    MRS     X0, SCTLR_EL1      ; Read SCTLR_EL1 into X0
    ORR     X0, X0, #(1<<26)   ; Set the UCI bit (bit 26) to 1
    MSR     SCTLR_EL1, X0      ; Write the modified value back to SCTLR_EL1
    RET
    ENDP

clear_uci PROC
    MRS     X0, SCTLR_EL1      ; Read SCTLR_EL1 into X0
    BIC     X0, X0, #(1<<26)   ; Clear the UCI bit (bit 26) to 0
    MSR     SCTLR_EL1, X0      ; Write the modified value back to SCTLR_EL1
    RET
    ENDP

    END