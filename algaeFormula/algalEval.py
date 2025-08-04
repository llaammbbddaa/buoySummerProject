# this is really just to see what "percent value" is most attributed with an algal bloom
# according to the gulf of mexico / florida shorline dataset, it is at about sixty eight percent

import pandas as pd

# --- CONFIGURE: path to your data file ---
DATA_PATH = '~/Downloads/floorRider.txt'
DELIMITER = '|'   # or ',' for CSV

# Reference values
REF_PH   = 8.0
REF_SAL  = 30.0
REF_TEMP = 25.0

def compute_score(row):
    pdiff_ph   = abs(row['pH'] - REF_PH)   / REF_PH
    pdiff_sal  = abs(row['Salinity'] - REF_SAL) / REF_SAL
    pdiff_temp = abs(row['Water Temperature'] - REF_TEMP) / REF_TEMP
    return 1.0 - (pdiff_ph + pdiff_sal + pdiff_temp)

def main():
    # --- load, skipping any extra header lines ---
    df = pd.read_csv(
        DATA_PATH,
        delimiter=DELIMITER,
        header=1,                # use second line as header if your first line is a dummy
        parse_dates=['SampleDate']
    )

    # --- compute bloom score ---
    df['BloomScore'] = df.apply(compute_score, axis=1)

    # --- group by bloom flag and compute stats ---
    grp = df.groupby('Algal Bloom Active')['BloomScore']
    summary = grp.agg([
        ('Mean',   'mean'),
        ('Median', 'median'),
        ('Mode',   lambda x: x.mode().iat[0] if not x.mode().empty else pd.NA),
        ('Min',    'min'),
        ('Max',    'max')
    ]).reset_index()

    # --- also compute Range as Max - Min ---
    summary['Range'] = summary['Max'] - summary['Min']

    # --- display ---
    print(summary.to_string(index=False))

if __name__ == '__main__':
    main()
