#!/usr/bin/env python
import sys
import pandas as pd

args = sys.argv

if len(args) != 2:
    print('Usage: ./summarize_error_log (error_log)')

    exit(0)

error_log_file = args[1]

error_log_df = pd.read_csv(error_log_file)

col = 'reason'
reason_df = error_log_df.groupby(col)[col].count()
print(reason_df)

col = 'same/diff_result'
modified_df = error_log_df.groupby(col)[col].count()
print(modified_df)
