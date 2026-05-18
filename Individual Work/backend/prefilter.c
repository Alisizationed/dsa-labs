#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/* =========================================================================
   Heuristic Pre-filter for Electoral Manipulation Detection
   
   Scores text 0.0–1.0 across several signal groups:
     - Emotional intensity   (caps ratio, exclamations, emotional words)
     - Fear appeals          (threat/danger vocabulary)
     - Disinformation        (fabrication markers, weasel phrases)
     - Propaganda            (enemy framing, absolute language)
     - Voter suppression     (voting process doubt/discouragement)
     - Unrealistic promises  (hyperbolic pledge vocabulary)
   
   Returns a JSON string so Python can parse it directly via ctypes.
   ========================================================================= */

#define MAX_RESULT 4096
#define MAX_WORD   256

/* ---- word lists --------------------------------------------------------- */

static const char *FEAR_WORDS[] = {
    "threat","danger","attack","destroy","collapse","chaos","crisis",
    "catastrophe","disaster","terrorism","invasion","takeover","corrupt",
    "criminal","violent","deadly","toxic","radical","extremist","evil",
    "enemy","plague","nightmare","tragedy","doom","catastrophic",NULL
};

static const char *DISINFORMATION_MARKERS[] = {
    "they don't want you to know","secret","hidden agenda","cover-up",
    "fake news","mainstream media lies","suppressed","censored",
    "what they're hiding","the truth is","wake up","sheeple",
    "deep state","globalist","rigged","stolen","fraud","fabricated",
    "hoax","conspiracy","they lied","exposed","whistleblow",NULL
};

static const char *PROPAGANDA_WORDS[] = {
    "traitor","parasite","vermin","filth","scum","coward","puppet",
    "regime","tyrant","dictator","overthrow","revolution","fight back",
    "take our country","real people","true patriots","elites","globalists",
    "establishment","corrupt media","enemy of the people","invasion",
    "replacement","great replacement","pure","unpatriotic",NULL
};

static const char *SUPPRESSION_WORDS[] = {
    "don't bother voting","vote doesn't count","pointless to vote",
    "election is rigged","your vote won't matter","stay home",
    "voting is useless","they'll steal it anyway","wrong polling",
    "polling closed","id required","you need passport","you can't vote",
    "ineligible","they will cheat",NULL
};

static const char *PROMISE_WORDS[] = {
    "guarantee","promise","will definitely","100%","absolutely will",
    "i swear","pledge","vow","eliminate all","fix everything","solve all",
    "end poverty","end crime","perfect","best ever","greatest","incredible",
    "like never before","nobody else can","only i can","i alone",NULL
};

static const char *EMOTIONAL_WORDS[] = {
    "outrage","shocking","unbelievable","incredible","disgusting","furious",
    "betrayal","shame","disgrace","humiliation","infuriating","devastating",
    "horrifying","appalling","scandalous","explosive","bombshell",
    "urgent","emergency","breaking","alert","warning","danger",NULL
};

/* ---- helpers ------------------------------------------------------------ */

static int str_contains_ci(const char *haystack, const char *needle) {
    if (!haystack || !needle) return 0;
    size_t hlen = strlen(haystack);
    size_t nlen = strlen(needle);
    if (nlen > hlen) return 0;
    for (size_t i = 0; i <= hlen - nlen; i++) {
        size_t j;
        for (j = 0; j < nlen; j++) {
            if (tolower((unsigned char)haystack[i+j]) !=
                tolower((unsigned char)needle[j])) break;
        }
        if (j == nlen) return 1;
    }
    return 0;
}

static int count_matches(const char *text, const char **words) {
    int count = 0;
    for (int i = 0; words[i] != NULL; i++) {
        if (str_contains_ci(text, words[i])) count++;
    }
    return count;
}

/* ratio of UPPERCASE letters among all alpha characters */
static double caps_ratio(const char *text) {
    int upper = 0, alpha = 0;
    for (const char *p = text; *p; p++) {
        if (isalpha((unsigned char)*p)) {
            alpha++;
            if (isupper((unsigned char)*p)) upper++;
        }
    }
    return alpha > 0 ? (double)upper / alpha : 0.0;
}

/* count occurrences of a single character */
static int count_char(const char *text, char c) {
    int n = 0;
    for (const char *p = text; *p; p++) {
        if (*p == c) n++;
    }
    return n;
}

/* approximate word count */
static int word_count(const char *text) {
    int count = 0, in_word = 0;
    for (const char *p = text; *p; p++) {
        if (isspace((unsigned char)*p)) { in_word = 0; }
        else if (!in_word) { in_word = 1; count++; }
    }
    return count > 0 ? count : 1;
}

/* clamp a double to [0,1] */
static double clamp01(double v) {
    return v < 0.0 ? 0.0 : (v > 1.0 ? 1.0 : v);
}

/* ---- scoring ------------------------------------------------------------ */

typedef struct {
    double emotional_intensity;
    double fear_appeal;
    double disinformation;
    double propaganda;
    double voter_suppression;
    double unrealistic_promises;
    double overall;
    int    should_escalate;   /* 1 = send to LLM, 0 = skip */
} HeuristicResult;

static HeuristicResult score_text(const char *text) {
    HeuristicResult r = {0};
    int words = word_count(text);

    /* -- emotional intensity -- */
    double cr      = caps_ratio(text);
    int excl       = count_char(text, '!');
    int emo_hits   = count_matches(text, EMOTIONAL_WORDS);
    double excl_rate = (double)excl / words;
    r.emotional_intensity = clamp01(
        cr * 0.4 +
        fmin(excl_rate * 5.0, 0.4) +
        fmin((double)emo_hits / 5.0, 0.2)
    );

    /* -- fear appeal -- */
    int fear_hits = count_matches(text, FEAR_WORDS);
    r.fear_appeal = clamp01((double)fear_hits / 6.0);

    /* -- disinformation -- */
    int dis_hits = count_matches(text, DISINFORMATION_MARKERS);
    r.disinformation = clamp01((double)dis_hits / 4.0);

    /* -- propaganda -- */
    int prop_hits = count_matches(text, PROPAGANDA_WORDS);
    r.propaganda = clamp01((double)prop_hits / 5.0);

    /* -- voter suppression -- */
    int supp_hits = count_matches(text, SUPPRESSION_WORDS);
    r.voter_suppression = clamp01((double)supp_hits / 2.0);

    /* -- unrealistic promises -- */
    int prom_hits = count_matches(text, PROMISE_WORDS);
    r.unrealistic_promises = clamp01((double)prom_hits / 4.0);

    /* -- overall (weighted) -- */
    r.overall = clamp01(
        r.emotional_intensity   * 0.15 +
        r.fear_appeal           * 0.20 +
        r.disinformation        * 0.25 +
        r.propaganda            * 0.20 +
        r.voter_suppression     * 0.15 +
        r.unrealistic_promises  * 0.05
    );

    /* escalate if overall >= 0.20 OR any single signal >= 0.50 */
    r.should_escalate = (
        r.overall               >= 0.20 ||
        r.fear_appeal           >= 0.55 ||
        r.disinformation        >= 0.25 ||
        r.propaganda            >= 0.25 ||
        r.emotional_intensity   >= 0.35 ||
        r.voter_suppression     >= 0.50
    ) ? 1 : 0;

    return r;
}

/*
 * analyse_text(text, out_buf, buf_size)
 *
 * Writes a JSON object into out_buf.
 * Returns 0 on success, -1 if the buffer is too small.
 */
int analyse_text(const char *text, char *out_buf, int buf_size) {
    if (!text || !out_buf || buf_size < 64) return -1;

    HeuristicResult r = score_text(text);

    int written = snprintf(out_buf, buf_size,
        "{"
        "\"emotional_intensity\":%.3f,"
        "\"fear_appeal\":%.3f,"
        "\"disinformation\":%.3f,"
        "\"propaganda\":%.3f,"
        "\"voter_suppression\":%.3f,"
        "\"unrealistic_promises\":%.3f,"
        "\"overall\":%.3f,"
        "\"should_escalate\":%s"
        "}",
        r.emotional_intensity,
        r.fear_appeal,
        r.disinformation,
        r.propaganda,
        r.voter_suppression,
        r.unrealistic_promises,
        r.overall,
        r.should_escalate ? "true" : "false"
    );

    return (written >= buf_size) ? -1 : 0;
}
