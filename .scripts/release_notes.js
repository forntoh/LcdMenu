const { context, github } = require("@actions/github");

async function generateReleaseNotes() {
  const { owner, repo } = context.repo;
  const currentTag = process.env.CURRENT_TAG;

  // Fetch all tags
  const { data: tags } = await github.repos.listTags({
    owner,
    repo,
    per_page: 100,
  });

  // Find the previous tag
  const currentTagIndex = tags.findIndex((tag) => tag.name === currentTag);
  const previousTag =
    currentTagIndex > 0 ? tags[currentTagIndex + 1].name : null;

  // Fetch PRs merged between previousTag and currentTag
  const { data: pulls } = await github.pulls.list({
    owner,
    repo,
    state: "closed",
    sort: "updated",
    direction: "desc",
    per_page: 100,
  });

  const categories = {
    feature: [],
    enhancement: [],
    bugfix: [],
    documentation: [],
    chore: [],
  };

  pulls
    .filter((pr) => pr.merged_at)
    .filter((pr) => {
      const mergedAt = new Date(pr.merged_at);
      const previousTagDate = previousTag
        ? new Date(
            tags.find((tag) => tag.name === previousTag).commit.committer.date
          )
        : new Date(0);
      const currentTagDate = new Date(
        tags.find((tag) => tag.name === currentTag).commit.committer.date
      );
      return mergedAt > previousTagDate && mergedAt <= currentTagDate;
    })
    .forEach((pr) => {
      pr.labels.forEach((label) => {
        if (categories[label.name]) {
          categories[label.name].push(
            `- ${pr.title} by @${pr.user.login} in ${pr.html_url}`
          );
        }
      });
    });

  const releaseNotes = Object.entries(categories)
    .filter(([_, notes]) => notes.length > 0)
    .map(
      ([category, notes]) =>
        `### ${
          category.charAt(0).toUpperCase() + category.slice(1)
        }\n${notes.join("\n")}`
    )
    .join("\n\n");

  return releaseNotes;
}

module.exports = generateReleaseNotes;
