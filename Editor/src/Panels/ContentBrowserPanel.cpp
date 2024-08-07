#include "qbpch.h"
#include "ContentBrowserPanel.h"

#include "Qbit/Project/Project.h"

#include <imgui/imgui.h>

namespace Qbit {

    ContentBrowserPanel::ContentBrowserPanel()
        : m_BaseDirectory(Project::GetAssetDirectory()), m_CurrentDirectory(m_BaseDirectory)
    {
        m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
        m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
    }

    static float padding = 16.0f;
    static float thumbnailSize = 128.0f;

    bool ContentBrowserPanel::IsTextureFile(const std::string& filepath)
    {
        std::string extension = filepath.substr(filepath.find_last_of('.') + 1);
        return (extension == "png" || extension == "jpg" || extension == "jpeg");
    }

    Ref<Texture2D> ContentBrowserPanel::LoadTexture(const std::string& filepath)
    {
        return Texture2D::Create(filepath);
    }

    Ref<Texture2D> ContentBrowserPanel::GetTextureForFile(const std::string& filepath)
    {
        if (m_TextureCache.find(filepath) != m_TextureCache.end())
        {
            return m_TextureCache[filepath];
        }

        Ref<Texture2D> texture = LoadTexture(filepath);
        m_TextureCache[filepath] = texture;
        return texture;
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin("Content Browser");

        if (ImGui::BeginTabBar("ContentBrowserTabs"))
        {
            if (ImGui::BeginTabItem("Content Browser"))
            {
                if (m_CurrentDirectory != std::filesystem::path(m_BaseDirectory))
                {
                    if (ImGui::Button("<-"))
                    {
                        m_CurrentDirectory = m_CurrentDirectory.parent_path();
                    }
                }

                float cellSize = thumbnailSize + padding;

                float panelWidth = ImGui::GetContentRegionAvail().x;
                int columnCount = (int)(panelWidth / cellSize);
                if (columnCount < 1)
                    columnCount = 1;

                ImGui::Columns(columnCount, 0, false);

                for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
                {
                    const auto& path = directoryEntry.path();
                    std::string filenameString = path.filename().string();

                    ImGui::PushID(filenameString.c_str());
                    Ref<Texture2D> icon;
                    if (directoryEntry.is_directory())
                    {
                        icon = m_DirectoryIcon;
                    }
                    else if (IsTextureFile(filenameString))
                    {
                        icon = GetTextureForFile(path.string());
                    }
                    else
                    {
                        icon = m_FileIcon;
                    }

                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                    ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

                    if (ImGui::BeginDragDropSource())
                    {
                        std::filesystem::path relativePath(path);
                        const wchar_t* itemPath = relativePath.c_str();
                        ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
                        ImGui::EndDragDropSource();
                    }

                    ImGui::PopStyleColor();
                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        if (directoryEntry.is_directory())
                            m_CurrentDirectory /= path.filename();
                    }
                    ImGui::TextWrapped(filenameString.c_str());

                    ImGui::NextColumn();

                    ImGui::PopID();
                }

                ImGui::Columns(1);

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Settings"))
            {
                ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
                ImGui::SliderFloat("Padding", &padding, 0, 32);

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }

}
