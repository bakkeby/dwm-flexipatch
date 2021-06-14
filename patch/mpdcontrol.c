#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>

#include <mpd/client.h>

#define MPDHOST "localhost"
#define MPDPORT 6600

struct mpd_connection *get_conn()
{
    struct mpd_connection *conn;

    conn = mpd_connection_new(MPDHOST, MPDPORT, 1000);

    if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
        fprintf(stderr, "Could not connect to mpd: %s\n", mpd_connection_get_error_message(conn));

        mpd_connection_free(conn);
        return NULL;
    }

    return conn;
}

void mpdchange(const Arg *direction)
{
    struct mpd_connection *conn;

    conn = get_conn();

    if (conn == NULL) {
        return;
    }

    if (direction->i > 0) {
        mpd_run_next(conn);
    }
    else {
        mpd_run_previous(conn);
    }

    mpd_connection_free(conn);
}

char *get_regerror(int errcode, regex_t *compiled)
{
    size_t length = regerror(errcode, compiled, NULL, 0);
    char *buffer = malloc(length);
    (void) regerror(errcode, compiled, buffer, length);

    return buffer;
}

void mpdcontrol()
{
    struct mpd_connection *conn;
    struct mpd_status *status;
    struct mpd_song *song;
    enum mpd_state state;

    const char *filename;

    regex_t expr;

    conn = get_conn();

    if (conn == NULL) {
        return;
    }

    status = mpd_run_status(conn);

    if (status == NULL) {
        fprintf(stderr, "Could not get mpd status: %s\n", mpd_status_get_error(status));

        mpd_status_free(status);
        mpd_connection_free(conn);
        return;
    }

    state = mpd_status_get_state(status);

    if (state == MPD_STATE_STOP || state == MPD_STATE_PAUSE) {
        mpd_run_play(conn);
        mpd_status_free(status);
        mpd_connection_free(conn);
    }
    else if (state != MPD_STATE_UNKNOWN) { //playing some music
        song = mpd_run_current_song(conn);

        if (song == NULL){
            fprintf(stderr, "Error fetching current song!\n");

            mpd_song_free(song);
            mpd_status_free(status);
            mpd_connection_free(conn);
            return;
        }

        filename = mpd_song_get_uri(song);

        int errcode = regcomp(&expr, "^[[:alnum:]]+://", REG_EXTENDED|REG_NOSUB);
        if (errcode != 0) {
            char *err = get_regerror(errcode, &expr);
            fprintf(stderr, "Could not compile regexp: %s\n", err);

            mpd_song_free(song);
            mpd_status_free(status);
            mpd_connection_free(conn);
            free(err);
            regfree(&expr);
            return;
        }

        int matchcode = regexec(&expr, filename, 0, NULL, 0);

        if (matchcode == 0) {
            if (strstr(filename, "file://") == filename) { //match just at the start of the filename
                //this means that mpd is playing a file outside the music_dir,
                //but on disk, so we can safely pause
                mpd_run_toggle_pause(conn);
            }
            else {
                mpd_run_stop(conn);
            }
        }
        else if (matchcode == REG_NOMATCH) {
            mpd_run_toggle_pause(conn);
        }
        else {
            char *err = get_regerror(matchcode, &expr);
            fprintf(stderr, "Error while matching regexp: %s\n", err);

            free(err);
        }

        regfree(&expr);
        mpd_song_free(song);
        mpd_status_free(status);
        mpd_connection_free(conn);
    }
}

