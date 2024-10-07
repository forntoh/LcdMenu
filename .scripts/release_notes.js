async function generateReleaseNotes(github, context) {
  const { owner, repo } = context.repo;
  const currentTag = process.env.CURRENT_TAG;

  console.log(`Current Tag: ${currentTag}`);

  // Fetch all tags
  const { data: tags } = await github.rest.repos.listTags({
    owner,
    repo,
    per_page: 10,
  });

  // Find the previous tag
  const currentTagIndex = tags.findIndex((tag) => tag.name === currentTag);
  const previousTag =
    currentTagIndex < tags.length - 1 ? tags[currentTagIndex + 1].name : null;

  const previousTagDate = previousTag
    ? new Date(
        tags.find((tag) => tag.name === previousTag)?.commit?.committer?.date ||
          0
      )
    : new Date(0);
  const currentTagDate = new Date(
    tags.find((tag) => tag.name === currentTag)?.commit?.committer?.date || 0
  );

  console.log(`Previous Tag: ${previousTag} (${previousTagDate})`);
  console.log(
    `Previous Tag: ${JSON.stringify(
      tags.find((tag) => tag.name === previousTag)
    )}`
  );
  console.log(`Current Tag: ${currentTag} (${currentTagDate})`);
  console.log(
    `Current Tag: ${JSON.stringify(
      tags.find((tag) => tag.name === currentTag)
    )}`
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
      const isInRange =
        mergedAt > previousTagDate && mergedAt <= currentTagDate;
      console.log(
        `PR #${pr.number} merged at ${mergedAt} is in range: ${isInRange}`
      );
      return isInRange;
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

  console.log(`Categories: ${JSON.stringify(categories, null, 2)}`);

  const releaseNotes = Object.entries(categories)
    .filter(([_, notes]) => notes.length > 0)
    .map(
      ([category, notes]) =>
        `### ${
          category.charAt(0).toUpperCase() + category.slice(1)
        }\n${notes.join("\n")}`
    )
    .join("\n\n");

  console.log(`Release Notes: ${releaseNotes}`);

  return releaseNotes;
}

module.exports = generateReleaseNotes;
