# SypexGeo module for Drupal

This module provides local based ip geolocation based on SypexGeo City database. It is fully free and has frequently updates.

## How to use

### Installation

At first you must download and install module as usual. Then you need to download SypexGeo PHP library and SypexGeo City database. You can do it in two ways:

#### First way - manualy

1. Go to [download page](https://sypexgeo.net/ru/download/).
2. Download "Sypex Geo для PHP 5.2+".

    2.1 Put `SxGeo.php` file to `PATH_TO_SITE_INSTALLATION/sites/all/libraries/sypexgeo`.
    
    2.2 File must be available at `/sites/all/libraries/sypexgeo/SxGeo.php`.
    
3. Download "Sypex Geo City" UTF-8 edition.

   3.1 Navigate to `/admin/config/regional/sypexgeo`.
   
   3.2 Upload `SxGeoCity.dat` file and save it.
   
4. You're done.

#### Second way - fastest

1. Install all you needs with just single command `drush sypexgeo`.
2. It will ask you, what you want to install, choose it and thats it.

![Drush Example](http://i.imgur.com/id0p8Dk.png)

Yes, you can also update SypexGeo database with drush. Just use second option when you want to make database fresh.


## PHP Examples

### Example 1

This example cover all your needs, it's wrapper for all functions in a module.

Params:

- `$ip` an array of IP's for check.
- `$type` a string determines which data you will receive. Can be: city, country, full (default). Scroll down to look what info it store.

~~~php
$result = sypexgeto_get_ip_info_multiple(array('127.0.0.1')), 'city');
$result = sypexgeto_get_ip_info_multiple(array('127.0.0.1', '127.0.0.2')));
~~~

### Example 2

Same as Example 1, just for single IP address. This is just wrapper for Example 1.

~~~php
$result = sypexgeto_get_ip_info('127.0.0.1'); // full
$result = sypexgeto_get_ip_info('127.0.0.1', 'city');
~~~

### Example 3

Returns full information about IP. Equals 'full' from first two examples.

~~~php
$result = sypexgeo_get_ip_city_full('127.0.0.1');
~~~

![Example 3](http://i.imgur.com/8J25rD4.png)

### Example 4

Returns information about city. Equals 'city' from first two examples.

~~~php
$result = sypexgeo_get_ip_city('127.0.0.1');
~~~

![Example 4](http://i.imgur.com/8h8NCE5.png)

### Example 5

Returns [ISO 3166-1 alpha-2](https://en.wikipedia.org/wiki/ISO_3166-1_alpha-2) (two symbols) country name. F.e. RU, US, GB. Equals 'country' from first two examples.

~~~php
$result = sypexgeo_get_ip_country('127.0.0.1');
~~~
