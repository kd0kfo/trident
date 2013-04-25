data = load '$input' as (stuff:chararray);
clean_data = distinct data;
store clean_data into '$output';
