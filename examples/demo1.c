void demo1(void)
/*! Sample code demonstrating the calculation of the position of the Sun using
 *  rectangular coordinates
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    Sky_DeltaTs     deltaTs;        /* Differences between timescales */
    Sky_SiteProp    site;           /* Properties of the site */
    Sky_SiteHorizon topo;           /* Sun position, as seen from the site */
    double          j2kUtc_d;       /* Time, expressed as a count of days */
    char            azStr[24], elStr[24];

    /* Set up the various delta T values */
    sky_initTime(37, 0, &deltaTs);

    /* Set up the fixed site data */
    sky_setSiteLocation(SITE_LATITUDE_deg,
                        SITE_LONGITUDE_deg,
                        SITE_HEIGHT_m,
                        &site);
    sky_setSiteTempPress(SITE_TEMPERATURE_degC,
                         SITE_PRESSURE_hPa,
                         &site);
    sky_setSiteTimeZone(SITE_TIMEZONE_h, &site);

    /* Get the current time in days since 2000-01-01 Greenwich noon */
    j2kUtc_d = sky_unixTimeToJ2kd(time(NULL));

    /* Get sun position */
    sun_nrelTopocentric(j2kUtc_d, &deltaTs, &site, &topo);

    /* Print out time and place */
    printf("Full sun calculation\n");
    skyio_printJ2kd(j2kUtc_d + site.timeZone_d);
    skyio_radToDmsStr(azStr, sizeof(azStr), topo.azimuth_rad, 2);
    skyio_radToDmsStr(elStr, sizeof(elStr), topo.elevation_rad, 2);
    printf(" Sun Azimuth: %s, Elevation: %s\n", azStr, elStr);
}
