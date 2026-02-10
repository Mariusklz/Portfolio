CC = gcc
COV_VERSION := $(shell $(CC) -dumpversion | cut -d. -f1)
ifeq ($(COV_VERSION),)
GCOV = gcov
else
GCOV = gcov-$(COV_VERSION)
endif

CFLAGS = -Wall -Wextra -O2 `pkg-config --cflags gtk4` -Iincludes
LDFLAGS = -lm `pkg-config --libs gtk4`
CFLAGS_TEST = -Wall -Wextra -O0 -fprofile-arcs -ftest-coverage -Iincludes -DNO_GUI `pkg-config --cflags gtk4`
LDFLAGS_TEST = -lm -fprofile-arcs -ftest-coverage `pkg-config --libs gtk4`

# Dossiers de build
BUILD_DIR = build
BUILD_TEST_DIR = $(BUILD_DIR)/tests

# Sources principales
SRC = src/main.c src/app.c src/gui.c src/ai.c src/network.c
OBJ = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRC))
TARGET = game

all: game

# Compilation du binaire principal (sans couverture)
game: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

# Compilation des fichiers .c vers build/
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Création des dossiers build si inexistants
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

###########################################################
# Compilation des tests avec couverture dans build/tests
$(BUILD_TEST_DIR)/test_app.o: tests/test_app.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS_TEST) -c $< -o $@

$(BUILD_TEST_DIR)/app_test_cov.o: src/app.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS_TEST) -c $< -o $@

$(BUILD_TEST_DIR)/ai_test_cov.o: src/ai.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS_TEST) -c $< -o $@

$(BUILD_TEST_DIR)/network_test_cov.o: src/network.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS_TEST) -c $< -o $@

$(BUILD_TEST_DIR)/app_test_cov_for_gui.o: src/app.c | $(BUILD_TEST_DIR)
	$(CC) -Wall -Wextra -O0 -fprofile-arcs -ftest-coverage -Iincludes `pkg-config --cflags gtk4` -c $< -o $@ # SANS -DNO_GUI

$(BUILD_TEST_DIR)/test_gui.o: tests/test_gui.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS_TEST) -c $< -o $@

$(BUILD_TEST_DIR)/gui_test_cov.o: src/gui.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS_TEST) -c $< -o $@

$(BUILD_TEST_DIR)/test_network.o: tests/test_network.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS_TEST) -c $< -o $@

# Création du dossier build/tests si nécessaire
$(BUILD_TEST_DIR):
	mkdir -p $(BUILD_TEST_DIR)

###########################################################
# Binaries tests
$(BUILD_TEST_DIR)/test_app: $(BUILD_TEST_DIR)/test_app.o $(BUILD_TEST_DIR)/app_test_cov.o
	$(CC) -o $@ $^ $(LDFLAGS_TEST)

$(BUILD_TEST_DIR)/test_gui: $(BUILD_TEST_DIR)/test_gui.o $(BUILD_TEST_DIR)/gui_test_cov.o \
    $(BUILD_TEST_DIR)/ai_test_cov.o $(BUILD_TEST_DIR)/network_test_cov.o $(BUILD_TEST_DIR)/app_test_cov_for_gui.o
	$(CC) -o $@ $^ $(LDFLAGS_TEST)

$(BUILD_TEST_DIR)/test_network: $(BUILD_TEST_DIR)/test_network.o $(BUILD_TEST_DIR)/network_test_cov.o
	$(CC) -o $@ $^ $(LDFLAGS_TEST)

# Compilation du test IA
$(BUILD_TEST_DIR)/test_ai.o: tests/test_ai.c | $(BUILD_TEST_DIR)
	$(CC) $(CFLAGS_TEST) -c $< -o $@

# Binaire test IA
$(BUILD_TEST_DIR)/test_ai: $(BUILD_TEST_DIR)/test_ai.o $(BUILD_TEST_DIR)/ai_test_cov.o
	$(CC) -o $@ $^ $(LDFLAGS_TEST)

###########################################################
# Lancer les tests et générer les rapports de couverture
tests: $(BUILD_TEST_DIR)/test_app $(BUILD_TEST_DIR)/test_gui $(BUILD_TEST_DIR)/test_network $(BUILD_TEST_DIR)/test_ai
	@echo "\033[1;34m===== Résultats des tests unitaires =====\033[0m"
	./$(BUILD_TEST_DIR)/test_app | grep --color=never -E "OK|passés" || true
	./$(BUILD_TEST_DIR)/test_gui | grep --color=never -E "OK|passés" || true
	-./$(BUILD_TEST_DIR)/test_network | grep --color=never -E "OK|passés" || true
	./$(BUILD_TEST_DIR)/test_ai | grep --color=never -E "OK|passés" || true
	gcov build/tests/app_test_cov.o -o build/tests/ > /dev/null
	gcov build/tests/gui_test_cov.o -o build/tests/ > /dev/null
	gcov build/tests/network_test_cov.o -o build/tests/ > /dev/null
	gcov build/tests/ai_test_cov.o -o build/tests/ > /dev/null
	@echo "\033[1;32m===== Résumé couverture =====\033[0m"
	grep "Lines executed" app.c.gcov || true
	grep "Lines executed" gui.c.gcov || true
	grep "Lines executed" network.c.gcov || true
	grep "Lines executed" ai.c.gcov || true
	cat app.c.gcov gui.c.gcov network.c.gcov > rapport.gcov
	cat ai.c.gcov >> rapport.gcov
	@echo "\033[1;36mRapport global généré dans rapport.gcov\033[0m"

clean:
	rm -rf $(BUILD_DIR) $(TARGET) *.gcda *.gcno *.gcov html/
	rm -f src/*.gcda src/*.gcno tests/*.gcda tests/*.gcno

.PHONY: all clean test docs

docs:
	doxygen Doxyfile