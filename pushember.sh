#!/bin/bash
echo "[x] Pushing Ember"
# pushember.sh – Pushes Ember to GitHub
REPO_URL="https://github.com/OwlyNest/Ember.git"

#Extract version info
VERSION="v0.2.0"
COMMIT_MSG=${*:-"Update on: Ember version $VERSION"}

#Clear stuck rebase state if it exists
if [ -d ".git/rebase-merge" ]; then
    echo "[x] Detected incomplete rebase. Cleaning up..."
    rm -rf .git/rebase-merge
fi

#Ensure this is a Git repo
if ! git rev-parse --is-inside-work-tree > /dev/null 2>&1; then
    echo "[x] Initializing new Git repo..."
    git init
fi

#Set correct remote URL
CURRENT_URL=$(git remote get-url origin 2>/dev/null)
if [[ "$CURRENT_URL" != "$REPO_URL" ]]; then
    echo "[x] Setting correct remote origin..."
    git remote remove origin 2>/dev/null
    git remote add origin "$REPO_URL"
fi

#Ensure it's on the main branch
BRANCH=$(git rev-parse --abbrev-ref HEAD)
if [[ "$BRANCH" == "HEAD" ]]; then
    echo "[x] Detached HEAD detected. Switching to main branch..."
    git checkout -B main
else
    git checkout main 2>/dev/null || git checkout -b main
fi

#Clean, commit, and sync
git add .
git commit -m "$COMMIT_MSG"

echo "[x] Pulling latest changes with rebase..."
if ! git pull --rebase origin main; then
    echo "[x] Rebase failed. Aborting..."
    git rebase --abort
    exit 1
fi

echo "[x] Pushing changes to origin/main..."
if ! git push -u origin main; then
    echo "[x] Push failed. Try pulling first or resolving conflicts."
    exit 1
fi

#Tag handling
if git rev-parse "$VERSION" >/dev/null 2>&1; then
    if git tag --points-at HEAD | grep -q "$VERSION"; then
        echo "[x] Tag '$VERSION' already exists on this commit."
    else
        echo "[x] Tag '$VERSION' exists but on a different commit. Skipping."
    fi
else
    echo "[x] Tagging release version $VERSION"
    git tag -a "$VERSION" -m "Release version $VERSION"
    git push origin "$VERSION"
fi

echo "[x] Ember push complete."
