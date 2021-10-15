# Crawler

A search engine generally contains three parts: a crawler, an indexer, and a query. 
The programs contained in this directory focus on the first feature, the crawler. 

For a search engine to work properly it must first know "all" of the websites, that
way when the user searches for something, the search engine will know exactly where
pages the desired pages are and what to match on the pages. This is predicated on have
a archive of "all" of the webpages. Thus, a crawler is needed to constantly crawl the web,
downloading and storing new webpages or updating old ones.

A simplier version of this described crawler is implemented here. The program will be given
a `root_url`. This represents the URL that the crawler must start with. It is will also be given
a `data_directory`, where the results of the crawler will be exported. Lastly, the crawler will
also be given a `max_depth` which represents the maximum number of webpages away from the root
the crawler must be able to crawl. 

To run the program we run the following series of commands:
```
$ make
$ ./crawler.x <ROOT_URL> <DATA_DIR> <MAX_DEPTH>
```
where `<ROOT_URL> <DATA_DIR> <MAX_DEPTH>` are the root url, data directory, and max depth arguments,
respectively. 

In particular, we note that the argument `max_depth` **must have a valid integer input**. The 
behavior of the program is undefined if non-integer inputs are given.

