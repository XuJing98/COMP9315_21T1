create table mySets (id integer primary key, iset intSet);
insert into mySets(1,'{1,2,3}');
insert into mySets(1,'{1,1,1}');
select * from mySets order by id;