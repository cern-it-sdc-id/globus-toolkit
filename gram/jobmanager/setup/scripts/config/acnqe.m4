AC_DEFUN(LAC_NQE_PROGS,
[
    GLOBUS_GRAM_JOB_MANAGER_PATH_PROG(GLOBUS_GRAM_JOB_MANAGER_QDEL, qdel, $1)
    GLOBUS_GRAM_JOB_MANAGER_PATH_PROG(GLOBUS_GRAM_JOB_MANAGER_QSTAT, qstat, $1)
    GLOBUS_GRAM_JOB_MANAGER_PATH_PROG(GLOBUS_GRAM_JOB_MANAGER_QSUB, qsub, $1)
]
)
