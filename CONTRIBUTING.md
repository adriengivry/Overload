# Contributing to Overload
First of all, thank you for your interest in Overload! Any contribution is welcome, including:

- Reporting a bug
- Submitting a fix
- Proposing new features
- Improving code quality

## We Develop with Github
We use GitHub to host code, track issues and feature requests, and accept pull requests.

## We Use [Github Flow](https://guides.github.com/introduction/flow/index.html), So All Code Changes Happen Through Pull Requests
Pull requests are the best way to propose changes to the codebase (we follow [Github Flow](https://guides.github.com/introduction/flow/index.html)). We actively welcome your pull requests.

To create a pull request:

1. Fork the repository.
2. Create your branch from `develop` (following naming conventions).
3. Review your code before submitting (conduct build and quality checks).
4. Create a pull request targeting the `develop` branch.

## Any contributions you make will be under the MIT Software License
In short, when you submit code changes, your contributions are understood to be under the same [MIT License](http://choosealicense.com/licenses/mit/) that covers the project. Feel free to contact the maintainers if that's a concern.

## Report bugs using Github's issues
We use GitHub issues to track bugs. Report a bug by opening a new issue, it's that easy!

## Coding Conventions
* Interfaces starts with `I`.
* Abstracts starts with `A`.
* Class names: `UpperCamelCase`.
* Public member variables: `lowerCamelCase`.
* Private member variables: `m_lowerCamelCase`.
* Public static variables: `UpperCamelCase`.
* Private static variables: `s_lowerCamelCase`.
* Function/Method arguments: `p_lowerCamelCase`.
* Function/Method names: `UpperCamelCase`.
* Constants: `kUpperCamelCase`.
* Class member variables are located at the bottom of the file.
* Avoid using macros to define constants; prefer using `constexpr`` instead.
* Tabs are preferred over spaces.
* Always end your files with an empty line.
* Avoid aligning variable names and values using tabulations.
* Scope blocks should start on a new line.
* Comment your functions, enums, classes, methods ([Javadoc style](https://en.wikipedia.org/wiki/Javadoc))

Some coding conventions may have been overlooked during the writing of this document, so always refer to the existing codebase.

## Thanks!
Thanks for being a part of the Overload Tech. team!
