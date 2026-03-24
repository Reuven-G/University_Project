#include "preprocessor.h"
#include "utils.h" /* משתמש בפונקציות עזר שלך */

/* עדכון לפונקציה הקיימת אצלך - הופך אותה למבצעית */
void process_macros_in_file(FILE *as_file, FILE *am_file) {
    char line[MAX_LINE_LENGTH];
    char *temp_line;
    
    /* רשימת המאקרואים שכבר התחלת להגדיר ב-H */
    MacroTable *table = create_macro_table(); 

    while (fgets(line, MAX_LINE_LENGTH, as_file)) {
        /* שימוש ב-utils שלך לניקוי רווחים בתחילת שורה */
        temp_line = skip_spaces(line); 

        if (is_macro_definition(temp_line)) {
            /* כאן נכנסת הלוגיקה שכבר התחלת לכתוב לשמירת המאקרו */
            handle_macro_definition(as_file, &table, temp_line);
            continue;
        }

        /* בדיקה אם השורה היא שם של מאקרו קיים */
        if (is_macro_in_table(table, temp_line)) {
            expand_macro(am_file, table, temp_line);
        } else {
            /* אם זו שורה רגילה - פשוט מעתיקים לקובץ החדש */
            fprintf(am_file, "%s", line);
        }
    }
    
    /* שחרור זיכרון כמו שמופיע אצלך ב-struct */
    free_macro_table(table);
}
