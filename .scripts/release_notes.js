function escapeSpecialChars(str) {
  const specialChars = /[\\`*_{}\[\]()#+\-!:.]/g;
  return str.replace(specialChars, "\\$&");
}

async function generateReleaseNotes(github, context) {
  const { owner, repo } = context.repo;
  const currentTag = process.env.CURRENT_TAG;

  // Fetch all tags
  const { data: tags } = await github.rest.repos.listTags({
    owner,
    repo,
    per_page: 100,
  });

  // Find the previous tag
  const currentTagIndex = tags.findIndex((tag) => tag.name === currentTag);
  const previousTag =
    currentTagIndex < tags.length - 1 ? tags[currentTagIndex + 1].name : null;

  console.log(`Current Tag: ${currentTag}`);
  console.log(`Previous Tag: ${previousTag}`);

  const getCommitDate = async (github, owner, repo, sha) => {
    const { data: commit } = await github.rest.repos.getCommit({
      owner,
      repo,
      ref: sha,
    });
    return new Date(commit.commit.committer.date);
  };

  const previousTagDate = previousTag
    ? await getCommitDate(
        github,
        owner,
        repo,
        tags.find((tag) => tag.name === previousTag).commit.sha
      )
    : new Date(0);

  const currentTagDate = await getCommitDate(
    github,
    owner,
    repo,
    tags.find((tag) => tag.name === currentTag).commit.sha
  );

  // Fetch PRs merged between previousTag and currentTag
  const { data: pulls } = await github.rest.pulls.list({
    owner,
    repo,
    state: "closed",
    sort: "updated",
    direction: "desc",
    per_page: 100,
  });

  const categoryNames = {
    feature: "New Features",
    enhancement: "Enhancements",
    bugfix: "Bug Fixes",
    chore: "Chore Updates",
    documentation: "Documentation Updates",
  };

  const categories = {
    feature: [],
    enhancement: [],
    bugfix: [],
    chore: [],
    documentation: [],
  };

  let hasBreakingChanges = false;

  pulls
    .filter((pr) => pr.merged_at)
    .filter((pr) => {
      const mergedAt = new Date(pr.merged_at);
      return mergedAt > previousTagDate && mergedAt <= currentTagDate;
    })
    .forEach((pr) => {
      const prEntry = `* ${escapeSpecialChars(pr.title)} by @${
        pr.user.login
      } in ${pr.html_url}`;

      pr.labels.forEach((label) => {
        if (label.name === "breaking-change") {
          hasBreakingChanges = true;
        }
        if (categories[label.name]) {
          categories[label.name].push(prEntry);
        } else if (!categories[label.name] && label.name === pr.labels[pr.labels.length - 1].name) {
          categories["chore"].push(prEntry);
        }
      });
    });

  console.log(`Categories: ${JSON.stringify(categories, null, 2)}`);

  const releaseNotes = Object.entries(categories)
    .filter(([_, notes]) => notes.length > 0)
    .map(
      ([category, notes]) =>
        `### ${
          categoryNames[category] ||
          category.charAt(0).toUpperCase() + category.slice(1)
        }\n${notes.join("\n")}`
    )
    .join("\n\n");

  const breakingChangesSection = hasBreakingChanges
    ? `### Breaking Changes\n\n- This release introduces breaking changes. Please review [the migration guide](https://lcdmenu.forntoh.dev/reference/migration/index.html) for details on how to update your code.\n\n`
    : "";

  const repoUrl = pulls.length > 0 ? pulls[0].base.repo.html_url : "";
  const fullChangelog = `**Full Changelog**: ${repoUrl}/compare/${previousTag}...${currentTag}`;
  return `${releaseNotes}\n\n${breakingChangesSection}\n\n${fullChangelog}`;
}

module.exports = generateReleaseNotes;
