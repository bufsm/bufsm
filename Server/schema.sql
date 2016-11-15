drop table if exists things;
create table things (
  thing_id integer primary key autoincrement,
  thing_name text not null,
  lat real,
  lon real
);
