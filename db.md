## Database

1. Create SQLite database file.

2. Create table.

3. (Optional) Create index for table.


### Example

```sql
CREATE TABLE air (
  id INTEGER PRIMARY KEY,
  ts INTEGER,
  hcho INTEGER,
  hcho_unit TEXT,
  tvoc INTEGER,
  tvoc_unit TEXT,
  co2 INTEGER,
  co2_unit TEXT
);

CREATE INDEX air_ts ON air(ts);
```
