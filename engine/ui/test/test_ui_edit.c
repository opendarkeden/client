/**
 * @file test_ui_edit.c
 * @brief Test ui_edit component
 */

#include "ui_edit.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

void test_init_and_clear() {
    printf("Test: init and clear\n");
    
    UI_Edit edit;
    ui_edit_init(&edit, 50);
    
    assert(edit.length == 0);
    assert(edit.cursor == 0);
    assert(edit.max_length == 50);
    assert(strcmp(edit.buffer, "") == 0);
    
    ui_edit_set_text(&edit, "Hello");
    assert(edit.length == 5);
    assert(strcmp(edit.buffer, "Hello") == 0);
    
    ui_edit_clear(&edit);
    assert(edit.length == 0);
    assert(strcmp(edit.buffer, "") == 0);
    
    printf("  ✓ Pass\n");
}

void test_insert_and_delete() {
    printf("Test: insert and delete\n");
    
    UI_Edit edit;
    ui_edit_init(&edit, 50);
    
    /* Insert characters */
    ui_edit_insert_char(&edit, 'H');
    ui_edit_insert_char(&edit, 'e');
    ui_edit_insert_char(&edit, 'l');
    ui_edit_insert_char(&edit, 'l');
    ui_edit_insert_char(&edit, 'o');
    
    assert(edit.length == 5);
    assert(edit.cursor == 5);
    assert(strcmp(edit.buffer, "Hello") == 0);
    
    /* Delete character */
    ui_edit_delete_char(&edit);
    assert(edit.length == 4);
    assert(edit.cursor == 4);
    assert(strcmp(edit.buffer, "Hell") == 0);
    
    /* Delete forward */
    ui_edit_cursor_home(&edit);
    ui_edit_delete_char_forward(&edit);
    assert(edit.length == 3);
    assert(strcmp(edit.buffer, "ell") == 0);
    
    printf("  ✓ Pass\n");
}

void test_cursor_movement() {
    printf("Test: cursor movement\n");
    
    UI_Edit edit;
    ui_edit_init(&edit, 50);
    ui_edit_set_text(&edit, "Hello World");
    
    /* Cursor starts at end */
    assert(edit.cursor == 11);
    
    /* Move left */
    ui_edit_cursor_left(&edit);
    assert(edit.cursor == 10);
    
    /* Move to home */
    ui_edit_cursor_home(&edit);
    assert(edit.cursor == 0);
    
    /* Move right */
    ui_edit_cursor_right(&edit);
    assert(edit.cursor == 1);
    
    /* Move to end */
    ui_edit_cursor_end(&edit);
    assert(edit.cursor == 11);
    
    /* Set cursor directly */
    ui_edit_set_cursor(&edit, 5);
    assert(edit.cursor == 5);
    
    printf("  ✓ Pass\n");
}

void test_insert_at_cursor() {
    printf("Test: insert at cursor position\n");
    
    UI_Edit edit;
    ui_edit_init(&edit, 50);
    ui_edit_set_text(&edit, "Helo");
    
    /* Insert 'l' at position 3 */
    ui_edit_set_cursor(&edit, 3);
    ui_edit_insert_char(&edit, 'l');
    
    assert(edit.length == 5);
    assert(strcmp(edit.buffer, "Hello") == 0);
    assert(edit.cursor == 4);
    
    printf("  ✓ Pass\n");
}

void test_max_length() {
    printf("Test: max length constraint\n");
    
    UI_Edit edit;
    ui_edit_init(&edit, 5);
    
    /* Fill to max */
    ui_edit_insert_char(&edit, '1');
    ui_edit_insert_char(&edit, '2');
    ui_edit_insert_char(&edit, '3');
    ui_edit_insert_char(&edit, '4');
    ui_edit_insert_char(&edit, '5');
    
    assert(edit.length == 5);
    
    /* Try to exceed max */
    int result = ui_edit_insert_char(&edit, '6');
    assert(result == 0);  /* Should fail */
    assert(edit.length == 5);
    
    printf("  ✓ Pass\n");
}

void test_password_mode() {
    printf("Test: password mode\n");
    
    UI_Edit edit;
    ui_edit_init(&edit, 50);
    
    /* Initially not password mode */
    assert(ui_edit_is_password_mode(&edit) == 0);
    
    /* Enable password mode */
    ui_edit_set_password_mode(&edit, 1);
    assert(ui_edit_is_password_mode(&edit) == 1);
    
    /* Disable password mode */
    ui_edit_set_password_mode(&edit, 0);
    assert(ui_edit_is_password_mode(&edit) == 0);
    
    printf("  ✓ Pass\n");
}

void test_digit_only_mode() {
    printf("Test: digit-only mode\n");
    
    UI_Edit edit;
    ui_edit_init(&edit, 50);
    ui_edit_set_digit_only_mode(&edit, 1);
    
    /* Allow digits */
    int r1 = ui_edit_insert_char(&edit, '1');
    int r2 = ui_edit_insert_char(&edit, '2');
    int r3 = ui_edit_insert_char(&edit, '3');
    
    assert(r1 == 1 && r2 == 1 && r3 == 1);
    assert(strcmp(edit.buffer, "123") == 0);
    
    /* Reject letters */
    int r4 = ui_edit_insert_char(&edit, 'A');
    assert(r4 == 0);
    assert(strcmp(edit.buffer, "123") == 0);
    
    printf("  ✓ Pass\n");
}

void test_readonly_mode() {
    printf("Test: read-only mode\n");
    
    UI_Edit edit;
    ui_edit_init(&edit, 50);
    ui_edit_set_text(&edit, "ReadOnly");
    
    /* Enable read-only */
    ui_edit_set_readonly_mode(&edit, 1);
    
    /* Try to modify */
    int r1 = ui_edit_insert_char(&edit, 'X');
    assert(r1 == 0);
    
    int r2 = ui_edit_delete_char(&edit);
    assert(r2 == 0);
    
    /* Text should be unchanged */
    assert(strcmp(edit.buffer, "ReadOnly") == 0);
    
    printf("  ✓ Pass\n");
}

void test_focus() {
    printf("Test: focus management\n");
    
    UI_Edit edit;
    ui_edit_init(&edit, 50);
    
    /* Initially not focused */
    assert(ui_edit_is_focused(&edit) == 0);
    
    /* Acquire focus */
    ui_edit_acquire_focus(&edit);
    assert(ui_edit_is_focused(&edit) == 1);
    
    /* Release focus */
    ui_edit_release_focus(&edit);
    assert(ui_edit_is_focused(&edit) == 0);
    
    printf("  ✓ Pass\n");
}

void test_keyboard_control() {
    printf("Test: keyboard control\n");
    
    UI_Edit edit;
    ui_edit_init(&edit, 50);
    ui_edit_acquire_focus(&edit);
    
    /* WM_CHAR for character input */
    ui_edit_keyboard_control(&edit, 0x0102, 'H', 0);  /* WM_CHAR, 'H' */
    ui_edit_keyboard_control(&edit, 0x0102, 'i', 0);
    
    assert(strcmp(edit.buffer, "Hi") == 0);
    
    /* WM_KEYDOWN for backspace */
    ui_edit_keyboard_control(&edit, 0x0100, 0x08, 0);  /* WM_KEYDOWN, VK_BACK */
    
    assert(strcmp(edit.buffer, "H") == 0);
    
    printf("  ✓ Pass\n");
}

void test_set_and_append() {
    printf("Test: set and append text\n");
    
    UI_Edit edit;
    ui_edit_init(&edit, 50);
    
    /* Set text */
    ui_edit_set_text(&edit, "Hello");
    assert(strcmp(edit.buffer, "Hello") == 0);
    assert(edit.cursor == 5);
    
    /* Append text */
    ui_edit_append_text(&edit, " World");
    assert(strcmp(edit.buffer, "Hello World") == 0);
    assert(edit.cursor == 11);
    
    printf("  ✓ Pass\n");
}

int main() {
    printf("========================================\n");
    printf("UI Edit Component Tests\n");
    printf("========================================\n\n");
    
    test_init_and_clear();
    test_insert_and_delete();
    test_cursor_movement();
    test_insert_at_cursor();
    test_max_length();
    test_password_mode();
    test_digit_only_mode();
    test_readonly_mode();
    test_focus();
    test_keyboard_control();
    test_set_and_append();
    
    printf("\n========================================\n");
    printf("All tests passed! ✓\n");
    printf("========================================\n");
    
    return 0;
}
