void demo4(void)
/*! Sample code using the greatly simplified calculation of the position
 *  of the Sun using interpolated coordinates (like demo2(), but also supporting
 *  multiple sites.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
{
    Sky_DeltaTs         deltaTs;        /* Differences between timescales */
    Sky_SiteProp        site[3];        /* Properties of the sites */
    Sky_SiteHorizon     topo;           /* Sun position, as seen from the site*/
    double              j2kUtc_d;       /* Time, expressed as a count of days */
    Sky_Times           atime;
    Sky_TrueEquatorial  approx;
    V3D_Vector          terInterV;// unit vector in Terrestrial Intermed Ref Sys
    char                azStr[24], elStr[24];
    int                 i, j;

    /* Get the current time in days since 2000-01-01 Greenwich noon */
    j2kUtc_d = sky_unixTimeToJ2kd(time(NULL));

    /* Set up the various delta T values. Values are taken from Bulletin A of
     * 23-Jul-2020, using the USNO prediction formula given on line 117:
     *       UT1-UTC = -0.2090 - 0.00011 (MJD - 59061) - (UT2-UT1)                 
     * to calculate deltaUT_s. (We need to convert time in J2KD format to MJD
     * form by adding 51544.5) */
    sky_initTimeDetailed(j2kUtc_d + 51544.5,
                         59061, -0.2090, -0.00011,
                         37, &deltaTs);

    /* Set up the fixed site data */
    sky_setSiteLocation(51.5074, -0.1278, 11.0, &site[0]);
    sky_setSiteTempPress(11.0, 1013.0, &site[0]);
    sky_setSiteTimeZone(0.0, &site[0]);

    sky_setSiteLocation(48.858093, 2.294694, 307.0, &site[1]);
    sky_setSiteTempPress(12.0, 974.0, &site[1]);
    sky_setSiteTimeZone(1.0, &site[1]);

    sky_setSiteLocation(41.9028, 12.4964, 21.0, &site[2]);
    sky_setSiteTempPress(15.0, 1010.0, &site[2]);
    sky_setSiteTimeZone(1.0, &site[2]);

    /* Set up the interpolation algorithm to calculate Sun positions  */
    skyfast_init(j2kUtc_d, 720, &deltaTs, &sun_nrelApparent);

    printf("Fast sun calculation at multiple sites\n");
    for (i = 0; i < 100; i++) {
        /* Get sun position using interpolation */
        j2kUtc_d = sky_unixTimeToJ2kd(time(NULL));
        sky_updateTimes(j2kUtc_d, &deltaTs, &atime);

        /* Get the approximate apparent coordinates by interpolation */
        skyfast_getApprox(atime.j2kTT_cy, &approx);

        /* Convert apparent coordinates to TIRS coordinates  */
        sky0_appToTirs(&approx.appCirsV,
                       atime.j2kUT1_d,
                       approx.eqEq_rad,
                       &terInterV);

        /* For each site, convert from TIRS coords to topocentric coords, and
           write out the result */
        for (j = 0; j < 3; j++) {
            sky_siteTirsToTopo(&terInterV, approx.distance_au, &site[j], &topo);

            printf("Site %d: ", j);
            skyio_printJ2kd(j2kUtc_d + site[j].timeZone_d);
            if (topo.azimuth_rad < 0.0) {
                topo.azimuth_rad += TWOPI;
            }
            skyio_radToDmsStr(azStr, sizeof(azStr), topo.azimuth_rad, 2);
            skyio_radToDmsStr(elStr, sizeof(elStr), topo.elevation_rad, 2);
            printf(" Sun Azimuth: %s, Elevation: %s\n", azStr, elStr);
        }


        sleep(1);       /* Just use a simple delay for this demo. */
    }
}


